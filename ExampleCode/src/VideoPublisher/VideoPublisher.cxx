/*********************************************************************************************
(c) 2005-2013 Copyright, Real-Time Innovations, Inc.  All rights reserved.
RTI grants Licensee a license to use, modify, compile, and create derivative works
of the Software.  Licensee has the right to distribute object form only for use with RTI
products.  The Software is provided �as is�, with no warranty of any type, including
any warranty for fitness for any purpose. RTI is under no obligation to maintain or
support the Software.  RTI shall not be liable for any incidental or consequential
damages arising out of the use or inability to use the software.
**********************************************************************************************/

#include <stdio.h>
#include <vector>
#include <iostream>

#include "connext_cpp_common.h"

#include "CommonInfrastructure/VideoSource.h"
#include "CommonInfrastructure/DDSCommunicator.h"
#include "CommonInfrastructure/OSAPI.h"

#include "VideoPublisherInterface.h"

#include <fcntl.h>
#include <string.h>
#include <glib-object.h>
#include "CommonInfrastructure/VideoOutput.h"
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/video/videooverlay.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

//additional bin for recording
#include <string.h>
#include <gst/gst.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>

using namespace std;
using namespace com::rti::media::generated;

//additional bin for recording
static GMainLoop *loop;
static GstElement *pipeline, *src, *encoder, *muxer, *sink;
static GstBus *bus;
static gboolean is_live;
static GstCaps *caps;
static gboolean wFlag;

//.........................................................................................................................//

static gboolean
message_cb (GstBus * bus, GstMessage * message, gpointer user_data)
{
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR:{
      GError *err = NULL;
      gchar *name, *debug = NULL;

      name = gst_object_get_path_string (message->src);
      gst_message_parse_error (message, &err, &debug);

      g_printerr ("ERROR: from element %s: %s\n", name, err->message);
      if (debug != NULL)
        g_printerr ("Additional debug info:\n%s\n", debug);

      g_error_free (err);
      g_free (debug);
      g_free (name);

      g_main_loop_quit (loop);
      break;
    }
    case GST_MESSAGE_WARNING:{
		GError *err = NULL;
		gchar *name, *debug = NULL;

		name = gst_object_get_path_string (message->src);
		gst_message_parse_warning (message, &err, &debug);

		g_printerr ("ERROR: from element %s: %s\n", name, err->message);
		if (debug != NULL)
		g_printerr ("Additional debug info:\n%s\n", debug);

		g_error_free (err);
		g_free (debug);
		g_free (name);
		break;
    }

	case GST_MESSAGE_BUFFERING:{
		gint percent = 0;
		gst_message_parse_buffering (message, &percent);
		g_print ("Buffering (%3d%%)\r", percent);

		if(is_live) break;

		if (percent < 100)
        	gst_element_set_state (pipeline, GST_STATE_PAUSED);
      	else
        	gst_element_set_state (pipeline, GST_STATE_PLAYING);
      	break;
	}
    case GST_MESSAGE_EOS:{
		g_print ("Got EOS\n");
		g_main_loop_quit (loop);
		gst_element_set_state (pipeline, GST_STATE_READY);
		g_main_loop_unref (loop);
		gst_object_unref (pipeline);
		exit(0);
		break;
	}
    default:
		break;
  }
  wFlag = true;
  return 0;
}

static gboolean webmFlag() {
  GstStateChangeReturn ret;

  pipeline = gst_pipeline_new(NULL);
  src = gst_element_factory_make("v4l2src", NULL);
  //converter = gst_element_factory_make("videoconverter", NULL);
  encoder = gst_element_factory_make("vp8enc", NULL);
  muxer = gst_element_factory_make("webmmux", NULL);
  sink = gst_element_factory_make("filesink", NULL);
  std::cout << "1" << std::endl;
  if (!pipeline || !src || !encoder || !muxer || !sink) {
    g_error("Failed to create elements");
    return -1;
  }
  std::cout << "2" << std::endl;
  /*output file location for sink element*/
  //g_object_set(src, "device","/dev/video0",NULL);
  g_object_set(sink, "location", "/root/rec.webm", NULL);

  /*adding elements to pipelines*/
  gst_bin_add_many(GST_BIN(pipeline), src, encoder, muxer, sink, NULL);

  /*specifying types of videos - capabilities*/
	caps = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, 640,  "height", G_TYPE_INT, 360, NULL);


	if(!gst_element_link_filtered(src, encoder, caps))
	{
		return FALSE;
	}

	gst_caps_unref(caps);

  if (!gst_element_link_many(encoder, muxer, sink, NULL)){
    g_error("Failed to link elements");
    return -2;
  }
//  	loop = g_main_loop_new(NULL, FALSE);
  g_object_set(encoder, "deadline", 2, NULL);
  /*build pipeline*/
  bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
  /*start playing*/
  ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
  if(ret == GST_STATE_CHANGE_FAILURE){
    gst_object_unref(pipeline);
    return -1;
  }else if (ret==GST_STATE_CHANGE_NO_PREROLL){
    is_live = TRUE;
    g_print("live streaming..");
  }
  gst_bus_add_signal_watch(bus);
  g_signal_connect(G_OBJECT(bus), "message", G_CALLBACK(message_cb), NULL);
  gst_object_unref(GST_OBJECT(bus));
  gst_element_set_state(pipeline, GST_STATE_PLAYING);
  g_print("Starting loop");
//  	g_main_loop_run(loop);
  wFlag = 1;
  return 0;

}
//--------------------------------------------------------------------------------------------------------//

void PrintHelp();

// ------------------------------------------------------------------------- //
//
// FrameHandler:
//
// A class that receives a notification from the GStreamer framework that
// a frame is available to be processed.  This copies that frame into a
// DDS VideoStream data type, and calls Write() to send it over the network.
//
// ------------------------------------------------------------------------- //
class FrameHandler : public EMDSFrameHandler
{

public:
	// --- Constructor ---
	FrameHandler() : _seqNum(0)
	{
		srand(time(NULL));
		_streamId = rand();
		std::cout << "Created frame publisher with unique stream ID: " <<
			_streamId << std::endl;

	}

	// ------------------------------------------------------------------------
	// Frame Ready:
	// When the video framework calls back this listener to tell it that a
	// frame is ready to be processed, this callback copies it into an RTI
	// Connext DDS data object and publishes it over DDS.
	virtual void FrameReady(void *obj, EMDSBuffer *buffer)
	{
		// Has a copy of the DDS interface that is used to publish data over
		// the network (or shared memory).
    //webmFlag(1, 50);
		VideoPublisherInterface *pubInterface =
			(VideoPublisherInterface *)obj;
      std::cout << "in frame ready," << std::endl;
		// Creates a VideoStream data object
		VideoStream *streamData =
				VideoStreamTypeSupport::create_data();
		std::cout << "Buffer: " << buffer << std::endl;
    std::cout << "streamData" << streamData<<std::endl;
		if (buffer->GetSize() > 0)
		{
			// Sets the frame information within the data to the correct size
			// based on the buffer size
			streamData->frame.ensure_length(
				buffer->GetSize(), buffer->GetSize());

			// Copy the data from the buffer into the object to be sent using
			// the middleware.
			streamData->frame.from_array(
				(DDS_Octet *)buffer->GetData(), buffer->GetSize());

			// Set the sequence number
			streamData->sequence_number = _seqNum;
			_seqNum++;
		}

		streamData->flags = buffer->GetFlags();
		streamData->stream_id = _streamId;

		// Sending the data over the network (or shared memory)
		pubInterface->Write(*streamData);

		// Cleaning up the data sample
		VideoStreamTypeSupport::delete_data(streamData);
	}

	virtual void EOSHandler(void *obj, EMDSBuffer *buffer)
	{
	}

private:

	// --- Private members ---
	int _seqNum;

	// Each video streamer has a different stream ID
	int _streamId;

};


// ------------------------------------------------------------------------- //
//
// CodecCompatibilityCheck:
//
// A class that acts as part of the interface between the video processing
// and the middleware.  This is called back from the VideoPublisherInterface
// when that object discovers a DataReader that is interested in the same
// Topic, stored in the constant VIDEO_TOPIC.
//
// This queries the video framework to see if the codecs are compatible with
// each other.
// ------------------------------------------------------------------------- //
class CodecCompatibilityCheck : public CodecCompatibleHandler
{
public:

	// --- Constructor ---
	CodecCompatibilityCheck(EMDSVideoSource *videoSource)
		: _source(videoSource), _discoveredCompatibleReader(false)
	{
	}

	// Calls the gstreamer framework to see if the subscribing app's codec
	// is compatible with what we are sending.  If not, we return false.
        // Note: this looks like a simple function but the implementation  has
        //       a side-effect of setting a private member -- this should be improved
	virtual bool CodecsCompatible(std::string codecString)
	{
      bool matches = false;
#if (VIDEODATA_MATCH_EMPTY_USERDATA == 1)
		matches = codecString.empty() || _source->IsMetadataCompatible(codecString);
      std::cout << "Matching compatibility: code was compiled to match with Subscribers without user_data" << std::endl;
#elif (VIDEODATA_MATCH_EMPTY_USERDATA == -1)
		matches = _source->IsMetadataCompatible(codecString);
      std::cout << "Matching compatibility: code was compiled to not match Subscribers without user_data" << std::endl;
#else /* VIDEODATA_MATCH_EMPTY_USERDATA */
#error Incorrect setup: VIDEODATA_MATCH_EMPTY_USERDATA should be defined and have the value -1 or 1
#endif /* VIDEODATA_MATCH_EMPTY_USERDATA */

		if (matches)
		{
			_discoveredCompatibleReader = true;
		}
		return _discoveredCompatibleReader;
	}

	bool DiscoveredCompatibleReader()
	{
		return _discoveredCompatibleReader;
	}

private:

	// --- Private members ---
	EMDSVideoSource *_source;
	bool _discoveredCompatibleReader;

};

// ----------------------------------------------------------------------------
//
// main function.  Prepares the video pipeline and the video publisher
// interface (the class that sends video data over RTI Connext DDS)
//
int main (int argc, char *argv[])
{
  gst_init (&argc, &argv);
  std::thread t1(webmFlag);
  t1.detach();

	cout << "--- Starting publisher application. --- " << endl;
	cout << "This application will read a video file, and publish it over RTI"
		<< " Connext DDS " << endl << "middleware" << endl;
	bool multicastAvailable = true;

	for (int i = 0; i < argc; i++)
	{
		if (0 == strcmp(argv[i], "--no-multicast"))
		{
			multicastAvailable = false;
		} else if (0 == strcmp(argv[i], "--help"))
		{
			PrintHelp();
			return 0;
		} else if (i > 0)
		{
			// If we have a parameter that is not the first one, and is not
			// recognized, return an error.
			cout << "Bad parameter: " << argv[i] << endl;
			PrintHelp();
			return -1;
		}

	}

  /*********************************************************************/

	// Set up paths for XML files.  The profiles are for applications that
	// have no multicast available at all, or that have multicast available
	// on the network.
	vector<string> xmlFiles;

	if (multicastAvailable)
	{
		// Adding the XML files that contain profiles used by this application
		xmlFiles.push_back(
			"file://../../../../src/Config/base_profile_multicast.xml");
		xmlFiles.push_back(
			"file://../../../../src/Config/video_stream_multicast.xml");
	}
	else
	{
		// Adding the XML files that contain profiles used by this application
		xmlFiles.push_back(
			"file://../../../../src/Config/base_profile_no_multicast.xml");
		xmlFiles.push_back(
			"file://../../../../src/Config/video_stream_no_multicast.xml");

	}

  std::string relativePath = "/root/rec.webm";
  std::cout << "reading from file"<<std::endl;
  //if (wFlag){
  //  std::string relativePath = "/home/hazel/Desktop/rec.webm";
  //  wFlag = FALSE;
  //}
	char fullPath[PATH_MAX];
	if (NULL == realpath(relativePath.c_str(), fullPath))
	{
		cout << "Error getting the file path" << endl;
	}
	EMDSVideoSource *videoSource = new EMDSVideoSource(
		fullPath);


	// If the video source was not created correctly, return an error.
	if (videoSource == NULL)
	{
		cout << "Failed to create video source" << endl;
		return -1;
	}

	// Initialize the video source, including opening the file
	if (videoSource->Initialize() != 0)
	{
		cout << "Failed to initialize video" << endl;
		return -1;
	}

	// Listener class to receive notifications of remote Video Subscriber apps
	// This checks if the remote application has a codec that is compatible
	// with what this application sends.
	CodecCompatibilityCheck compatibilityCheck(videoSource);

	// The Video Publisher Interface is responsible for:
	// 1. Publishing video data over RTI Connext DDS
	// 2. Receiving discovery notifications about video subscribers that have
	//    been found.  If those have the correct Topic, and also contain
	//    user_data QoS with codec information, we check whether they have
	//    a codec that is compatible with what we are sending.  We only send if
	//    the subscriber app has a compatible codec to us.  Note that this can
	//    be used for a variety of uses, such as determining video quality, etc

	try
	{
		VideoPublisherInterface videoInterface(xmlFiles,
			&compatibilityCheck);

		// Callback from the gstreamer framework that is providing us with video
		// frames.  This frameHandler uses the VideoPublisherInterface's Write()
		// method to write frames over the network.
		FrameHandler *frameHandler = new FrameHandler();
		videoSource->SetNewFrameCallbackHandler(
				frameHandler,
				(void *)&videoInterface);
		std::cout << "-----videioSource " <<videoSource<< std::endl;
		//not declared Metadata ! std::cout << "Metadata  " << Metadata << '\n';

		// Wait for compatible DataReaders to come online
#if 0
		while (!compatibilityCheck.DiscoveredCompatibleReader())
		{
			cout << "Waiting for a compatible video subscriber to come "
				<< "online" << endl;
			OSThread::Sleep(2,0);
		}
#endif

		// If we have found a compatible Video Subscriber, we start publishing.
		videoSource->Start();

		// Loop forever here
		while (1)
		{
			OSThread::Sleep(0, 100000000);
		}

	}
	catch (string message)
	{
		cout << "Application exception" << message << endl;
	}


}

void PrintHelp()
{
	cout << "Valid options are: " << endl;
	cout <<
		"    --no-multicast" <<
		"       Do not use any multicast, including for discovery"
		<< endl <<
		"                         " <<
		"(note you must edit XML config to include IP" << endl <<
		"                         " <<
		"addresses)"
		<< endl;

}

