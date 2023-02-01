#pragma once

#pragma once
#include <cstdio>
#include <array>

template <std::size_t N>
constexpr auto
decode_base85(
	const char( &input )[ N ]
) noexcept
{
	std::array<char, N * 4 / 5> out{};

	constexpr auto decode85Byte = [ ](
		char c
		) constexpr -> unsigned int
	{
		return c >= '\\' ? c - 36 : c - 35;
	};

	for (std::size_t i = 0, j = 0; i < N - 1; i += 5, j += 4)
	{
		unsigned int tmp = decode85Byte( input[ i ] ) + 85 * (decode85Byte( input[ i + 1 ] ) + 85 * (decode85Byte( input[ i + 2 ] ) + 85 * (decode85Byte( input[ i + 3 ] ) + 85 * decode85Byte( input[ i + 4 ] ))));
		out[ j ] = ((tmp >> 0) & 0xFF);
		out[ j + 1 ] = ((tmp >> 8) & 0xFF);
		out[ j + 2 ] = ((tmp >> 16) & 0xFF);
		out[ j + 3 ] = ((tmp >> 24) & 0xFF);
	}

	return out;
}

// File: 'blur_x' (600 bytes)
// Exported using binary_to_compressed_c.cpp
inline constexpr auto blur_x = decode_base85(
	"%/P:vP>$(#>T$<8?####Q$###%/P:v%####?####$&###J$###h####&####$####t#########.$###%####$####:$########tn=j=$8HlEQ2TuY3l:$#%),##$#########0g'WC"
	"`-.:CVjSuY&5>##%),##$#########C,.33UnSc;'AViF6JrEH<Sk:0+bVe=K&&PDlf1eGdfX1F$*fUCs'el1K>,C5AH3I3b48(#$QUV$)%XHVd;#K7#####X/'.7`7r'7$QUV$*%XHV"
	"d:i[7bmhf6##########D5YY#NSi.L,nHS[D5YY#_9r:Q0=XHVi>uu#^XF0LdfIl[[BA`V&5YY#W.]/Lpu$GV+>uu#PYr.LOV%JLou$GV&5YY#Q`%/Lpv*PV(>uu#Sf./L5hJcLdfIl["
	"(>uu#Rf./L4_/GLdfIl[&5YY#Y.]/Lqu$GV+>uu#RYr.LQV%JLou$GV&5YY#S`%/Lpv*PV(>uu#Uf./L7hJcLdfIl[(>uu#Tf./L6_/GLdfIl[i>uu#_XF0L4_/GL[BA`Vi>uu#`XF0L"
	"5_/GL[BA`Vi>uu#aXF0L6_/GL[BA`Vi>uu#bXF0L7_/GL[BA`V+>uu#W(S/L5_/GLpw0YV+G:;$W(S/L3_/GLpx6cV5_/GL+G:;$V(S/L4_/GLpw0YV5_/GL+G:;$V(S/L7_/GLqv*PV"
	"4_/GL+G:;$U(S/L6_/GLqv*PV4_/GL&5YY#fqF0L3_/GL#),##" );

// File: 'blur_y' (656 bytes)
// Exported using binary_to_compressed_c.cpp
inline constexpr auto blur_y = decode_base85(
	"%/P:vP>$(#>T$<8?####Q$###%/P:v%####?####$&###J$###h####&####$####t#########.$###%####$####:$########tn=j=$8HlEQ2TuY3l:$#%),##$#########0g'WC"
	"Qk;nDhpF/#&5>##%),##$#########C,.33UnSc;'AViF6JrEH<Sk:0+bVe=K&&PDlf1eGdfX1F$*fUCs'el1K>,C5AH3I3b48(#$QUV$)%XHVd;#K7NSi.LX/'.7`7r'7$QUV$*%XHV"
	"d:i[7bmhf6##########D5YY#NSi.L,nHS[D5YY#_9r:Q0=XHVi>uu#^XF0LdfIl[[BA`V&5YY#W.]/Lpu$GV+>uu#PYr.LOV%JLou$GV&5YY#Q`%/LP].JL&5YY#PYr.Lpv*PV(>uu#"
	"Rf./L4_/GLdfIl[&5YY#QYr.L)[-S[+G:;$R`%/Lou$GVOV%JL)]3][&5YY#Y.]/Lqu$GV+>uu#Sl7/LQV%JLou$GV&5YY#S`%/LP_:]L&5YY#RYr.Lpv*PV(>uu#Tf./L6_/GLdfIl["
	"&5YY#SYr.L)[-S[+G:;$T`%/Lou$GVQV%JL)]3][i>uu#_XF0L4_/GL[BA`Vi>uu#`XF0L5_/GL[BA`Vi>uu#aXF0L6_/GL[BA`Vi>uu#bXF0L7_/GL[BA`V+>uu#V(S/L4_/GLpw0YV"
	"+G:;$V(S/L3_/GLpx6cV4_/GL+G:;$V(S/L5_/GLpw0YV4_/GL+G:;$V(S/L6_/GLqv*PV4_/GL+G:;$U(S/L7_/GLqv*PV4_/GL&5YY#fqF0L3_/GL#),##" );

inline constexpr auto avatar_t = decode_base85(
	"=pxq9_8v7&0C/2'K(HH=CD4;-CD4;-.,###AYPROA7YY#o.s*>5-mRP`56JdY6=%UrBTe4SCL]^nv_mO_B8=C3&IVf_0:va(rlnP_8dFeW?Z8cc$dNGvA:YebK$Wb*.;4QC3X1cst53Z"
	"RLgmF8VrBR:KkrCLTY'6llv/dW6_Vb%o(4Qh`M(f[6$Wb`M3tN(a._]4C7(T>+T2G<tfb>3lH;b9n<.S'g+$fO4fx`$r,VOI'xkb?PBhQ4HU;N)JpT`^1k5Yti'fIIbA)[*RE%UO'20H"
	"PdNHYr_CFS$Hu:D?S`^RHhX8cAENt8Cu-2>TpS;Wh6[-SSnDr`:u.]`^JjpOK.L=_xP'F^2+<)KN6Y&]Joj5Y6Y4fH6c?JYmI1FS+)78ETh0=T3L]8NQb2[Bol;&IU*RuB@eW-4f_?bb"
	"UWZ8cxkLkQ['Nr`I?Z8c_ME9O);F@^uGbe]1u@xL424w^JmcYV9@<dJDS5TVPvZNP*p1ZC))DBRX$Z*IkB,J>1m$>J`3%>Brbt]7ishMDr,_H>56^*5Uo1]?NL;m;#g&FTi%A)fdN-Wb"
	"Z/JMQL^Kic>J2`_ec3XN%g7C].1+>a9I4YM2=.kbV^5>VL$r,I$j45WbxF*SFUgQFJep9U7k0QO<YBBAaajGP;5M2F*[QZ80=A^?O:qo:H#vb5REp_Yc8w/QsEAALbG'IYL.U$AW>1ZM"
	"4FIkblhGNZnT[LRNW0McLwp7c;U#4PBZYbe-.ZNZ_G[<N``/X_-]7]`F3P#MZ-9HZ@gCVWkPF.I,^&sUc_CFS[jM4GT&4lW_Y]-RvVh7Eb^;RVB6_mOfg=:Dqd%nMjt.sM`N<DAt$6pL"
	"'jP5EC,pt9ABU<Agj=K=L,m+5xp4Xe9E.NP:MR[M4i-=gm#3TcQR6DJAf`bQ+:ooNFf'b@U`T$rg$t4oFrc)>v7G>ReIb'5X5m8F@GdS^;4:9FKn13FiXQkhdr)aU>b;1>J'pM<`8,=("
	"TWUBi3A]cae$BlEn8cscm3mS`Kf@bgsU(K'uLjjrZT61<ne^6Ya@+ZrK=UG`)t,1<rwX0.cH<GoFq9P$ID6YVLn@aJZnu+qVOJgLgaK8oM]g0&,%$]3.HMWWjr_TJ&GCq.A_4de2itXN"
	"ac*wHht]F%9Z,bBYSshO[CJ>:I`^3;vAfdDGt).n1.,a><DmW#0#L7AZ$snW'm35*Zr&9`9=Ss8ThJJ4;m)b_8#DTL@?V@@nmlE+g(4_OZIG+8Q2n$`g[D%^jt*fA&=>GZGg;%DFAJg+"
	"0*P1+:Ko?cDF+kC(?_:^VlhM%PDY)IeQBR8O-bOEhcU^33B92W(<'Q-]jLD2)e?WYVYfZUeU8+%+gYbJG`qWLG)BBt_,tIqG#Tw.W^5,M;`ZuTxtCpKv#V2B=NHe[1VA<+e8Tk<118gS"
	"5cgsYVwq#p;e@tlR9x..YJq'A](e8MNe:4ZxFjLV8diMqoh^r`wjF>Pc0^kDnC_p#8?`obc83:gmmg,'uf7Y9c(NtNCO'<TOGa[H3-^rrZR*,+^=OMEojqW`+/MD+3QD2Lrh6[Fd8^Dl"
	"OC&@n%UZ$%r*uB;5o,[?d[Dn/f#87Do-V]fZgL#A'9Cp^t9+4F<I#x>1sNZ:7ZvYh%-HEf`$f.&<+NV&8Be5[QpPrC5aH#Q[^pYiJUPIT015^lvdgk+4E<PZSjM*oBgghR;v,dijmvms"
	"^,bUepL%^7,WS<2,J?\?X_p7VTIhc&^4cQ]nsu]]GFSLnBRUF-mA5jmGKvfFke5kvO)r5ePpNGf*Psc[.P1+QFp;#?D980.83uET8vDjTqn&RnE[n]:0:O%_@gs1RW[IN-XJCc+^dX0uG"
	"8isLt6CHUC,<OSBaTrdMiTLh?uO4):^H__pAg)h9QXI)Xaq(Y99i)%Mgv]9UE'Tj('f&@@Z(2]lo_vN6cdskDoFZsmNg[2E&.aH+mIJ?I_@?UPB/sS/+Y^nStjKOn$&0IPR[ur7L@LAd"
	"^7OcuAt0*6H2A,jDfu,5Ij1qi[(*Y0%o>Ab't?=uDmZ*[_dH9EjVO9,bjd?OpUnTW0wo(g9*PkbxfiXN$r7K`H7Y]4*UR;*uUXen3E$,]H7='rk&B5e=hWcg_S&?Z82@coKSaLoqO:oU"
	"[rU?=dHm^5-F]Um#0HPU/5Ysc_xLJp%Hh18mD?O@_l[V@6m#W9gDSh-TY<.3)kuHL?MP=N#..*-Y]Y).<*%##]Kg99QaYG8[W'##" );

inline constexpr auto avatar_ct = decode_base85(
	"=pxq9_8v7&0C/2'K(HH=CD4;-CD4;-.,###AYPRO?%###6/s*>][t4:201b,LT3&5W.Sr860c*,LV0/PW8f79+r0BHM%N#64>ld=_BVfFqbZf<ZUE_?@r;-J>jWUL02R'?[YE%.J-ic3"
	"gF0>`*bkX:+1R)>Tr$b>Y%pX/cY,?6eht=UGH`j4O)sL;=s=H=l0h$RhD_lXjQDCS_Eo]BGVvDH//W_IA]$s:-uqc=:9&K<mvRqTxA8XK0EqEH%)#MOCLmtU4I&TWuv%x8r<r4Xd=p*]"
	"fMcCKm)BMEgVLNYZ.2$9_vvk:R,GpQa5YY#I=$w76(p`Cq8JVj$SPj`a#&'C[:H8PaPG:vR@7Lj$m%J]+)muC;lKYum>Lu-E#vp/&&K))AMca#4i6ZCv&;'>e8_;JUvmZ]<`F,#b2mou"
	"@oo)AnqTf'pJKbF%N'JqXGXEfbk[5P%1W3H1#,TuEiKaZ(VUM*T?$Pf:86X-=&]uKI;?b0OehN^flb$tP<XQr31t$U6$2,0]$isCl7eb9iUs_E`[@^Ib=>3#SVjNpGDZQS6hG[CK5q3&"
	"+8G7BnU@2J,%q,_s[]0T%KgU1xxM$/,xNGXJ83di<Q1RJ0Fabe6$[)?Vp@IVZ6a;>3eD,ba5eh^?1pq30P?Q<%w,3(U:w^>cm3ENE$iW5BoM'p$MSH-n:cC;+[6e1EO_(`^+?k#ZMKJP"
	"+_9BilQECUOC/Qd]Kb$j^lpPi^e$M_*m`d&o.sR2-k<:b<vLvT=Qh>->5(?e;)(2K/1v&[8+k)iOcD+d5+k:M)`HkImIVB$W3wB#:3.,,TXG&j'nsA0tQd:AJk'?-(HxvS&v@B`7t'Ao"
	"l(')(98au8j'O4h`@4.*?&5UQcr5>k+bF]lPSG[_L2M-JGT]@P9jrLrljmWga8-iolx4lZ%o8$kxHpj5:eoB`+ki?*'e)0$9CX:@'w$/#Hen7t7;G:F49@:(#%to0qHsIUQjin@bAZS6"
	"KEUmEN6c8(VcRl_5i/<V=>)P&)(`pP64cDg`bZek8?IM,XW0LQ=n^UC`h#Duk[9qLaWB,:]tQKlJ4,[jQIaw:HL-NqKc$.ZRe#'kE$a8l4t^w#55L9RC3tan#m7YYg:&/9j/0M,S:vxa"
	"oVY5LcuWEt]'=KK=ClX,nqrZ`e:XkH=[[cRYPjeA=8B=WPw:Da0fIT9rcw85e%###Ub90<d$bsAP$###" );