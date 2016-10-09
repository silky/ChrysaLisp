%include 'inc/func.inc'
%include 'inc/load.inc'
%include 'class/class_symbol.inc'
%include 'class/class_stream.inc'
%include 'class/class_lisp.inc'

	def_function class/lisp/repl_read_sym
		;inputs
		;r0 = lisp object
		;r1 = stream
		;r2 = next char
		;outputs
		;r0 = lisp object
		;r1 = symbol
		;r2 = next char

		const char_space, ' '
		const char_lb, '('
		const char_rb, ')'
		const char_quote, "'"

		ptr this, stream, symbol
		pubyte relloc, buffer
		ulong char

		push_scope
		retire {r0, r1, r2}, {this, stream, char}

		slot_function sys_load, statics
		assign {@_function_.ld_statics_reloc_buffer}, {relloc}
		assign {relloc}, {buffer}

		loop_while {char > char_space && char != char_lb && char != char_rb && char != char_quote}
			assign {char}, {*buffer}
			assign {buffer + 1}, {buffer}
			static_call stream, read_char, {stream}, {char}
		loop_end

		;intern the symbol
		static_call symbol, create_from_buffer, {relloc, buffer - relloc}, {symbol}
		static_call lisp, sym_intern, {this, symbol}, {symbol}

		eval {this, symbol, char}, {r0, r1, r2}
		pop_scope
		return

	def_function_end