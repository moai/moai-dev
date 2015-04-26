#!/usr/bin/perl -w
# vi: set ts=4:
#

$debug = 0;
$pedantic = 0;

sub get_flags
{
	my @list = @insns;
	my $exts = {};
	my $s;

	$debug && print "function: $func\n";
	foreach $insn (@list) {
		#$debug && print "checking instruction \"$insn\"\n";
		$insn =~ m/^(\w+)\s+(.*)$/;
		$opcode = $1;
		$regs = $2;
		if (grep { /^$opcode$/ } @normal_list) {
			$debug && print "  $opcode: normal\n";
		}elsif (grep { /^$opcode$/ } @cmov_list) {
			#$exts->{"cmov"} = 1;
			$debug && print "  $opcode: cmov\n";
		}elsif (grep { /^$opcode$/ } @mmxsse_list) {
			if (grep { /\%xmm/ } $regs) {
				$exts->{"sse2"} = 1;
				$debug && print "  $opcode: sse2\n";
			} else {
				$exts->{"mmx"} = 1;
				$debug && print "  $opcode: mmx\n";
			}
		}elsif (grep { /^$opcode$/ } @mmx_list) {
			$exts->{"mmx"} = 1;
			$debug && print "  $opcode: mmx\n";
		}elsif (grep { /^$opcode$/ } @mmx_ext_list) {
			$exts->{"mmxext"} = 1;
			$debug && print "  $opcode: mmxext\n";
		}elsif (grep { /^$opcode$/ } @_3dnow_list) {
			$exts->{"3dnow"} = 1;
			$debug && print "  $opcode: 3dnow\n";
		}elsif (grep { /^$opcode$/ } @_3dnow_ext_list) {
			$exts->{"3dnowext"} = 1;
			$debug && print "  $opcode: 3dnowext\n";
		}elsif (grep { /^$opcode$/ } @sse_list) {
			$exts->{"sse"} = 1;
			$debug && print "  $opcode: sse\n";
		}elsif (grep { /^$opcode$/ } @sse2_list) {
			$exts->{"sse2"} = 1;
			$debug && print "  $opcode: sse2\n";
		}elsif (grep { /^$opcode$/ } @sse3_list) {
			$exts->{"sse3"} = 1;
			$debug && print "  $opcode: sse3\n";
		}elsif (grep { /^$opcode$/ } @ssse3_list) {
			$exts->{"ssse3"} = 1;
			$debug && print "  $opcode: ssse3\n";
		}else {
			print "FIXME:\t\"$opcode\",\n";
			$error = 1;
		}
	}
	if (!$pedantic) {
        if ($exts->{"sse3"}) {
            $exts->{"sse2"} = 1;
        }
        if ($exts->{"sse2"}) {
            $exts->{"sse"} = 1;
            $exts->{"mmxext"} = 1;
        }
    }
	$s = join(" ",sort(keys(%$exts)));
	$funcs->{"$func"} = $s;
	$debug && print "  FLAGS: $s\n";
}

sub check
{
	foreach $insn (@normal_list) {
		if (grep { /^$insn$/ } @mmx_list) {
			print "FIXME: $insn is in mmx_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @mmx_ext_list) {
			print "FIXME: $insn is in mmx_ext_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @_3dnow_list) {
			print "FIXME: $insn is in _3dnow_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @_3dnow_ext_list) {
			print "FIXME: $insn is in _3dnow_ext_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @sse_list) {
			print "FIXME: $insn is in sse_list\n";
			$error = 1;
		} elsif (grep { /^$insn$/ } @sse2_list) {
			print "FIXME: $insn is in sse2_list\n";
			$error = 1;
		}
	}
}

# this list is not complete
@normal_list = (
	"add", 
	"addl", 
	"and", 
	"andl", 
	"bswap",
	"call", 
	"cld", 
	"cltd", 
	"cmp", 
	"cmpb", 
	"cmpl", 
	"cwtl", 
	"dec", 
	"decl", 
	"fabs", 
	"fadd", 
	"faddl", 
	"faddp", 
	"fadds", 
	"fchs",
	"fcom",
	"fcomp",
	"fcompp",
	"fdiv",
	"fdivl",
	"fdivr",
	"fdivrl", 
	"fdivrs",
	"fdivs",
	"fiaddl",
	"fimull",
	"fild", 
	"fildl", 
	"fildll", 
	"fistp", 
	"fistpl", 
	"fistpll", 
	"fld",
	"fld1",
	"fldcw", 
	"fldl", 
	"flds", 
	"fldt",
	"fldz", 
	"fmul", 
	"fmull", 
	"fmulp", 
	"fmuls", 
	"fnstcw", 
	"fnstsw", 
	"frndint",
	"fsqrt", 
	"fstl", 
	"fstp", 
	"fstpl", 
	"fstps", 
	"fstpt",
	"fsts", 
	"fsub", 
	"fsubl", 
	"fsubp", 
	"fsubr", 
	"fsubrl", 
	"fsubrp", 
	"fsubrs",
	"fsubs",
	"fucom", 
	"fucomi", 
	"fucomp", 
	"fucompp", 
	"fxch", 
	"idiv",
	"imul", 
	"imulb",
	"inc", 
	"incl", 
	"ja", 
	"jae", 
	"jb",
	"jbe", 
	"je", 
	"jg", 
	"jge", 
	"jl", 
	"jle", 
	"jmp", 
	"jne", 
	"jns", 
	"jp", 
	"js", 
	"lea", 
	"leave", 
	"mov", 
	"movb", 
	"movl", 
	"movsbl", 
	"movsbw", 
	"movswl", 
	"movsww", 
	"movw",
	"movzbl", 
	"movzbw", 
	"movzwl", 
	"mul",
	"mulb", 
	"neg", 
	"nop", 
	"not", 
	"or", 
	"pop", 
	"push", 
	"pushl", 
	"rep",
	"repz", 
	"ret", 
	"rol", 
	"ror", 
	"sahf", 
	"sar", 
	"sarl", 
	"setl",
	"shl", 
	"shll",
	"shr", 
	"shrl", 
	"sub", 
	"subl", 
	"test", 
	"testb", 
	"testl", 
	"xchg",
	"xor", 
);

@cmov_list = (
	"cmova",
	"cmovae",
	"cmovb",
	"cmovg", 
	"cmovge", 
	"cmovl",
	"cmovle", 
	"cmovbe",
	"fcmovbe",
	"fcmovnbe",
);

# verified
@mmx_list = (
	"emms",
);

# verified
@_3dnow_list = (
	"femms",
	"pavgusb",
	"pf2id",
	"pfacc",
	"pfadd",
	"pfcmpeq",
	"pfcmpge",
	"pfcmpgt",
	"pfmax",
	"pfmin",
	"pfmul",
	"pfrcp",
	"pfrcpit1",
	"pfrcpit2",
	"pfrsqit1",
	"pfrsqrt",
	"pfsub",
	"pfsubr",
	"pi2fd",
	"pmulhrw",
	"prefetch",
	"prefetchw"
);

# verified
@_3dnow_ext_list = (
	"pf2iw",
	"pfnacc",
	"pfpnacc",
	"pi2fw",
	"pswapd"
);

# verified
@mmx_ext_list = (
	"maskmovq",
	"movntq",
	"pavgb",
	"pavgw",
	"pextrw",
	"pinsrw",
	"pmaxsw",
	"pmaxub",
	"pminsw",
	"pminub",
	"pmovmskb",
	"pmulhuw",
	"prefetchnta",
	"prefetch0",
	"prefetch1",
	"prefetch2",
	"psadbw",
	"pshufw",
	"sfence"
);

# verified
@sse_list = (
	"addps",
	"addss",
	"andnps",
	"andps",
	"cmpps",
	"cmpss",
	"comiss",
	"cvtpi2ps",
	"cvtps2pi",
	"cvtsi2ss",
	"cvtss2si",
	"cvttps2pi",
	"cvttss2si",
	"divps",
	"divss",
	"fxrstor",
	"fxsave",
	"ldmxcsr",
	"maxps",
	"maxss",
	"minps",
	"minss",
	"movaps",
	"movhlps",
	"movhps",
	"movlhps",
	"movlps",
	"movmskps",
	"movss",
	"movups",
	"mulps",
	"mulss",
	"orps",
	"rcpps",
	"rcpss",
	"rsqrtps",
	"rsqrtss",
	"shufps",
	"sqrtps",
	"sqrtss",
	"stmxcsr",
	"subps",
	"subss",
	"ucomiss",
	"unpckhps",
	"unpcklps",
	"xorps"
);

@mmxsse_list = (
	"movd",
	"movq",
	"packssdw",
	"packsswb",
	"packuswb",
	"paddb",
	"paddd",
	"paddsb",
	"paddsw",
	"paddusb",
	"paddusw",
	"paddw",
	"pand",
	"pandn",
	"pcmpeqb",
	"pcmpeqd",
	"pcmpeqw",
	"pcmpgtb",
	"pcmpgtd",
	"pcmpgtw",
	"pmaddwd",
	"pmulhw",
	"pmullw",
	"pmulhuw",
	"por",
	"pslld",
	"psllq",
	"psllw",
	"psrad",
	"psraw",
	"psrld",
	"psrlq",
	"psrlw",
	"psubb",
	"psubd",
	"psubsb",
	"psubsw",
	"psubusb",
	"psubusw",
	"psubw",
	"punpckhbw",
	"punpckhdq",
	"punpckhwd",
	"punpcklbw",
	"punpckldq",
	"punpcklwd",
	"pxor",
);

# verified
@sse2_list = (
	"addpd",
	"addsd",
	"andnpd",
	"andpd",
	"cmppd",
	"cmpsd",
	"comisd",
	"cvtpi2pd",
	"cvtpd2pi",
	"cvtsi2sd",
	"cvtsd2si",
	"cvttpd2pi",
	"cvttsd2si",
	"cvtpd2ps",
	"cvtps2pd",
	"cvtsd2ss",
	"cvtss2sd",
	"cvtps2dq",
	"cvttpd2dq",
	"cvtdq2pd",
	"cvtps2dq",
	"cvttps2dq",
	"cvtdq2ps",
	"divpd",
	"divsd",
	"maxpd",
	"maxsd",
	"minpd",
	"minsd",
	"movapd",
	"movhpd",
	"movlpd",
	"movmskpd",
	"movsd",
	"movupd",
	"mulpd",
	"mulsd",
	"orpd",
	"shufpd",
	"sqrtpd",
	"sqrtsd",
	"subpd",
	"subsd",
	"ucomisd",
	"unpckhpd",
	"unpcklpd",
	"xorpd",
	#"movd",
	"movdqa",
	"movdqu",
	"movq2dq",
	"movdq2q",
	#"movq",
	#"packssdw",
	#"packsswb",
	#"packuswb",
	"paddq",
	"padd",
	"padds",
	"paddus",
	#"pand",
	#"pandn",
	"pavgb",
	"pavgw",
	"pcmpeq",
	"pcmpgt",
	"pextrw",
	"pinsrw",
	#"pmaddwd",
	"pmaxsw",
	"pmaxub",
	"pminsw",
	"pminub",
	"pmovmskb",
	"pmulhuw",
	#"pmulhw",
	#"pmullw",
	"pmuludq",
	#"por",
	"psadbw",
	"pshuflw",
	"pshufhw",
	"pshufd",
	"pslldq",
	"psll",
	"psra",
	"psrldq",
	"psrl",
	"psubq",
	"psub",
	"psubs",
	"psubus",
	"punpckh",
	"punpckhqdq",
	"punpckl",
	"punpcklqdq",
	#"pxor",
	"maskmovdqu",
	"movntpd",
	"movntdq",
	"movnti",
	"pause",
	"lfence",
	"mfence",
);

@sse3_list = (
	"addsubpd",
	"addsubps",
	"haddpd",
	"haddps",
	"hsubpd",
	"hsubps",
	"lddqu",
	"movddup",
	"movshdup",
	"movsldup",
	"fisttp"
);

@ssse3_list = (
	"psignb",
	"psignw",
	"psignd",
	"pabsb",
	"pabsw",
	"pabsd",
	"palignr",
	"pshufb",
	"pmulhrsw",
	"pmaddubsw",
	"phsubw",
	"phsubd",
	"phsubsw",
	"phaddw",
	"phaddd",
	"phaddsw"
);

#@clflush_list = (
#	"clflush",
#);

$funcs = {};

$ARGV=shift @ARGV;
@output=`objdump -j .text -dr $ARGV`;

check();

$error = 0;
@insns = ();
while($_=shift @output){
	chomp;
	if(m/^0[0-9a-fA-F]+\s<[\.\w@]+>:$/){
		$f = $_;
		$f =~ s/^0[0-9a-fA-F]+\s<([\.\w]+)>:$/$1/;

		if (@insns) {
			get_flags ();
		}

		$func = $f;

		@insns = ();
		$debug && print "$func:\n";

	} elsif(m/^[\s0-9a-f]+:\s[\s0-9a-f]{20}\s+([\w]+\s.*)$/){
		push @insns, $1;
		#print "  $1\n";
	} elsif(m/^[\s0-9a-f]+:\s[\s0-9a-f]{2,20}\s$/){
		# ignore
	} elsif (m/^$/) {
	} elsif (m/^Disassembly of section/) {
	} elsif (m/\sfile format\s/) {
	} else {
		print "FIXME: $_\n";
		$error = 1;
	}
}

@source = `./list-impls`;
while ($_ = shift @source) {
	chomp;
	if (m/^([\w\.]+):\s*([\w\s*]*)/) {
		$func = $1;
		$flags = $2;

		$xflags = $funcs->{$func};
		if (1) {
			if ($flags ne $xflags) {
				print "$func: \"$flags\" should be \"$xflags\"\n";
			}
		} else {
			print "FIXME: function \"$func\" has no disassembly\n";
			$error = 1;
		}
	} else {
		print "FIXME: bad match: $_\n";
	}
}

exit $error;


