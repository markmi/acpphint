#### other_src_used/README.md
`acpphint` is a variation on the original HINT benchmark, but written in C++ and with various choices specific to my use.

While, overall, `acpphint` is licensed as GPLv2 (not earlier, not later), building the `acpphint` related code does build some OpenBSD-vintage-ISC licensed source files, these are under `other_src_used/` . Some of these object files are linked into the acpphint related programs.  There is one program (cpp_clock_info_main) which only uses `other_src_used/` source code (including itself). The ISC licensed source code and these object files are not derivatives of the original HINT benchmark code and have other potential uses.

See the `other_src_used/LICENSE.md` for the OpenBSD-vintage-ISC license text that applies to just the source files in `other_src_used/` . Each distributed `other_src_used/` C++ source file also contains the OpenBSD-vintage-ISC license text.

Note: Part of the intent in choosing the OpenBSD-vintage-ISC license is to be compatible with GPLv2 for making overall GPLv2 licensed results.
