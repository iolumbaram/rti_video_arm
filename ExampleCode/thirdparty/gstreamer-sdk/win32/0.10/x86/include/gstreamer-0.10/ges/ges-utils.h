/* GStreamer Editing Services
 * Copyright (C) 2010 Edward Hervey <edward.hervey@collabora.co.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _GES_UTILS
#define _GES_UTILS

#include <glib-object.h>
#include <gst/gst.h>
#include <ges/ges-types.h>

G_BEGIN_DECLS

GESTimeline * ges_timeline_new_audio_video (void);

G_END_DECLS

#endif /* _GES_UTILS */

