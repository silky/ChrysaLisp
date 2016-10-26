%include 'inc/func.inc'
%include 'class/class_vector.inc'
%include 'class/class_string.inc'
%include 'class/class_boxed_long.inc'
%include 'class/class_error.inc'
%include 'class/class_lisp.inc'

	def_func class/lisp/func_char
		;inputs
		;r0 = lisp object
		;r1 = args
		;outputs
		;r0 = lisp object
		;r1 = value

		ptr this, args, value
		ulong length

		push_scope
		retire {r0, r1}, {this, args}

		devirt_call vector, get_length, {args}, {length}
		if {length == 1}
			func_call vector, get_element, {args, 0}, {value}
			if {value->obj_vtable == @class/class_boxed_long}
				func_call boxed_long, get_value, {value}, {length}
				func_call string, create_from_cstr, {&length}, {value}
			else
				func_call error, create, {"(char int) not a number", args}, {value}
			endif
		else
			func_call error, create, {"(char int) wrong number of args", args}, {value}
		endif

		eval {this, value}, {r0, r1}
		pop_scope
		return

	def_func_end