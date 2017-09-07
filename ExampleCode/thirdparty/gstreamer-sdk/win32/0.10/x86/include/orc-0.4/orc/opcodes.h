UNARY_SB(absb, "ORC_ABS(%s)")
BINARY_SB(addb, "%s + %s")
BINARY_SB(addssb, "ORC_CLAMP_SB(%s + %s)")
BINARY_UB(addusb, "ORC_CLAMP_UB((orc_uint8)%s + (orc_uint8)%s)")
BINARY_SB(andb, "%s & %s")
BINARY_SB(andnb, "(~%s) & %s")
BINARY_SB(avgsb, "(%s + %s + 1)>>1")
BINARY_UB(avgub, "((orc_uint8)%s + (orc_uint8)%s + 1)>>1")
BINARY_SB(cmpeqb, "(%s == %s) ? (~0) : 0")
BINARY_SB(cmpgtsb, "(%s > %s) ? (~0) : 0")
UNARY_SB(copyb, "%s")
BINARY_SB(maxsb, "ORC_MAX(%s, %s)")
BINARY_UB(maxub, "ORC_MAX((orc_uint8)%s, (orc_uint8)%s)")
BINARY_SB(minsb, "ORC_MIN(%s, %s)")
BINARY_UB(minub, "ORC_MIN((orc_uint8)%s, (orc_uint8)%s)")
BINARY_SB(mullb, "(%s * %s) & 0xff")
BINARY_SB(mulhsb, "(%s * %s) >> 8")
BINARY_UB(mulhub, "((orc_uint32)(orc_uint8)%s * (orc_uint32)(orc_uint8)%s) >> 8")
BINARY_SB(orb, "%s | %s")
BINARY_SB(shlb, "%s << %s")
BINARY_SB(shrsb, "%s >> %s")
BINARY_UB(shrub, "((orc_uint8)%s) >> %s")
UNARY_SB(signb, "ORC_CLAMP(%s,-1,1)")
BINARY_SB(subb, "%s - %s")
BINARY_SB(subssb, "ORC_CLAMP_SB(%s - %s)")
BINARY_UB(subusb, "ORC_CLAMP_UB((orc_uint8)%s - (orc_uint8)%s)")
BINARY_SB(xorb, "%s ^ %s")

UNARY_SW(absw, "ORC_ABS(%s)")
BINARY_SW(addw, "%s + %s")
BINARY_SW(addssw, "ORC_CLAMP_SW(%s + %s)")
BINARY_UW(addusw, "ORC_CLAMP_UW((orc_uint16)%s + (orc_uint16)%s)")
BINARY_SW(andw, "%s & %s")
BINARY_SW(andnw, "(~%s) & %s")
BINARY_SW(avgsw, "(%s + %s + 1)>>1")
BINARY_UW(avguw, "((orc_uint16)%s + (orc_uint16)%s + 1)>>1")
BINARY_SW(cmpeqw, "(%s == %s) ? (~0) : 0")
BINARY_SW(cmpgtsw, "(%s > %s) ? (~0) : 0")
UNARY_SW(copyw, "%s")
BINARY_SW(maxsw, "ORC_MAX(%s, %s)")
BINARY_UW(maxuw, "ORC_MAX((orc_uint16)%s, (orc_uint16)%s)")
BINARY_SW(minsw, "ORC_MIN(%s, %s)")
BINARY_UW(minuw, "ORC_MIN((orc_uint16)%s, (orc_uint16)%s)")
BINARY_SW(mullw, "(%s * %s) & 0xffff")
BINARY_SW(mulhsw, "(%s * %s) >> 16")
BINARY_UW(mulhuw, "((orc_uint32)((orc_uint16)%s) * (orc_uint32)((orc_uint16)%s)) >> 16")
BINARY_SW(orw, "%s | %s")
BINARY_SW(shlw, "%s << %s")
BINARY_SW(shrsw, "%s >> %s")
BINARY_UW(shruw, "((orc_uint16)%s) >> %s")
UNARY_SW(signw, "ORC_CLAMP(%s,-1,1)")
BINARY_SW(subw, "%s - %s")
BINARY_SW(subssw, "ORC_CLAMP_SW(%s - %s)")
BINARY_UW(subusw, "ORC_CLAMP_UW((orc_uint16)%s - (orc_uint16)%s)")
BINARY_SW(xorw, "%s ^ %s")

UNARY_SL(absl, "ORC_ABS(%s)")
BINARY_SL(addl, "%s + %s")
BINARY_SL(addssl, "ORC_CLAMP_SL((orc_int64)%s + (orc_int64)%s)")
BINARY_UL(addusl, "ORC_CLAMP_UL((orc_int64)(orc_uint32)%s + (orc_int64)(orc_uint32)%s)")
BINARY_SL(andl, "%s & %s")
BINARY_SL(andnl, "(~%s) & %s")
BINARY_SL(avgsl, "((orc_int64)%s + (orc_int64)%s + 1)>>1")
BINARY_UL(avgul, "((orc_uint64)(orc_uint32)%s + (orc_uint64)(orc_uint32)%s + 1)>>1")
BINARY_SL(cmpeql, "(%s == %s) ? (~0) : 0")
BINARY_SL(cmpgtsl, "(%s > %s) ? (~0) : 0")
UNARY_SL(copyl, "%s")
BINARY_SL(maxsl, "ORC_MAX(%s, %s)")
BINARY_UL(maxul, "ORC_MAX((orc_uint32)%s, (orc_uint32)%s)")
BINARY_SL(minsl, "ORC_MIN(%s, %s)")
BINARY_UL(minul, "ORC_MIN((orc_uint32)%s, (orc_uint32)%s)")
BINARY_SL(mulll, "(%s * %s) & 0xffffffff")
BINARY_SL(mulhsl, "((orc_int64)%s * (orc_int64)%s) >> 32")
BINARY_UL(mulhul, "((orc_uint64)(orc_uint32)%s * (orc_uint64)(orc_uint32)%s) >> 32")
BINARY_SL(orl, "%s | %s")
BINARY_SL(shll, "%s << %s")
BINARY_SL(shrsl, "%s >> %s")
BINARY_UL(shrul, "((orc_uint32)%s) >> %s")
UNARY_SL(signl, "ORC_CLAMP(%s,-1,1)")
BINARY_SL(subl, "%s - %s")
BINARY_SL(subssl, "ORC_CLAMP_SL((orc_int64)%s - (orc_int64)%s)")
BINARY_UL(subusl, "ORC_CLAMP_UL((orc_int64)(orc_uint32)%s - (orc_int64)(orc_uint32)%s)")
BINARY_SL(xorl, "%s ^ %s")

UNARY_SQ(copyq, "%s")
BINARY_SQ(cmpeqq, "(%s == %s) ? (~0) : 0")
BINARY_SQ(cmpgtsq, "(%s > %s) ? (~0) : 0")
BINARY_SQ(andq, "%s & %s")
BINARY_SQ(andnq, "(~%s) & %s")
BINARY_SQ(orq, "%s | %s")
BINARY_SQ(xorq, "%s ^ %s")
BINARY_SQ(addq, "%s + %s")
BINARY_SQ(subq, "%s - %s")
BINARY_SQ(shlq, "%s << %s")
BINARY_SQ(shrsq, "%s >> %s")
BINARY_UQ(shruq, "((orc_uint64)%s) >> %s")

UNARY_BW(convsbw, "%s")
UNARY_BW(convubw, "(orc_uint8)%s")
UNARY_WB(convwb, "%s")
UNARY_WB(convhwb, "((orc_uint16)%s)>>8")
UNARY_WB(convssswb, "ORC_CLAMP_SB(%s)")
UNARY_WB(convsuswb, "ORC_CLAMP_UB(%s)")
UNARY_WB(convusswb, "ORC_MIN((orc_uint16)%s,ORC_SB_MAX)")
UNARY_WB(convuuswb, "ORC_MIN((orc_uint16)%s,ORC_UB_MAX)")

UNARY_WL(convswl, "%s")
UNARY_WL(convuwl, "(orc_uint16)%s")
UNARY_LW(convlw, "%s")
UNARY_WB(convhlw, "((orc_uint32)%s)>>16")
UNARY_LW(convssslw, "ORC_CLAMP_SW(%s)")
UNARY_LW(convsuslw, "ORC_CLAMP_UW(%s)")
UNARY_LW(convusslw, "ORC_CLAMP((orc_uint32)%s,0,ORC_SW_MAX)")
UNARY_LW(convuuslw, "ORC_CLAMP_UW((orc_uint32)%s)")

UNARY_LQ(convslq, "%s")
UNARY_LQ(convulq, "(orc_uint32)%s")
UNARY_LW(convql, "%s")
UNARY_LW(convsssql, "ORC_CLAMP_SL(%s)")
UNARY_LW(convsusql, "ORC_CLAMP_UL(%s)")
UNARY_LW(convussql, "ORC_CLAMP_SL((orc_uint64)%s)")
UNARY_LW(convuusql, "ORC_CLAMP_UL((orc_uint64)%s)")

BINARY_BW(mulsbw, "%s * %s")
BINARY_BW(mulubw, "((orc_uint8)%s) * ((orc_uint8)%s)")
BINARY_WL(mulswl, "%s * %s")
BINARY_WL(muluwl, "((orc_uint16)%s) * ((orc_uint16)%s)")
BINARY_LQ(mulslq, "((orc_int64)%s) * ((orc_int64)%s)")
BINARY_LQ(mululq, "((orc_uint64)((orc_uint32)%s)) * ((orc_uint64)((orc_uint32)%s))")

UNARY_UW(swapw, "ORC_SWAP_W(%s)")
UNARY_UL(swapl, "ORC_SWAP_L(%s)")
UNARY_UQ(swapq, "ORC_SWAP_Q(%s)")

BINARY_F(addf, "%s + %s")
BINARY_F(subf, "%s - %s")
BINARY_F(mulf, "%s * %s")
BINARY_F(divf, "%s / %s")
UNARY_F(sqrtf, "sqrt(%s)")
BINARY_FL(cmpeqf, "(%s == %s) ? (~0) : 0")
BINARY_FL(cmpltf, "(%s < %s) ? (~0) : 0")
BINARY_FL(cmplef, "(%s <= %s) ? (~0) : 0")

BINARY_D(addd, "%s + %s")
BINARY_D(subd, "%s - %s")
BINARY_D(muld, "%s * %s")
BINARY_D(divd, "%s / %s")
UNARY_D(sqrtd, "sqrt(%s)")
BINARY_DQ(cmpeqd, "(%s == %s) ? ORC_UINT64_C(~0) : 0")
BINARY_DQ(cmpltd, "(%s < %s) ? ORC_UINT64_C(~0) : 0")
BINARY_DQ(cmpled, "(%s <= %s) ? ORC_UINT64_C(~0) : 0")
