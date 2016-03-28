%include 'inc/func.inc'
%include 'inc/gui.inc'

	fn_function gui/region_translate
		;inputs
		;r0 = region listhead pointer
		;r8 = x translation
		;r9 = y translation
		;trashes
		;r0

		;run through source region list
		loop_flist_forward r0, r0, r0
			vp_add r8, [r0 + gui_rect_x]
			vp_add r9, [r0 + gui_rect_y]
			vp_add r8, [r0 + gui_rect_x1]
			vp_add r9, [r0 + gui_rect_y1]
		loop_end
		vp_ret

	fn_function_end
