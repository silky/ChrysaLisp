%include "func.inc"
%include "list.inc"
%include "gui.inc"

	struc DRAW_VIEW
		DRAW_VIEW_CTX:	resq 1
		DRAW_VIEW_VIEW:	resq 1
		DRAW_VIEW_NEXT:	resq 1
		DRAW_VIEW_SIZE:
	endstruc

	fn_function "gui/view_draw"
	draw_view:
		;inputs
		;r0 = ctx
		;r1 = view object
		;trashes
		;r0-r3, r5-r15

		vp_sub DRAW_VIEW_SIZE, r4
		vp_cpy r0, [r4 + DRAW_VIEW_CTX]
		vp_cpy r1, [r4 + DRAW_VIEW_VIEW]

		;draw myself
		vp_lea [r1 + GUI_DIRTY_LIST], r2
		vp_cpy r2, [r0 + GUI_CTX_DIRTY_REGION]
		vp_call [r1 + GUI_VIEW_DRAW]
		vp_cpy [r4 + DRAW_VIEW_CTX], r0
		vp_cpy [r4 + DRAW_VIEW_VIEW], r1

		;draw child views
		loop_list_forwards r1 + GUI_VIEW_LIST, r2, r1
			vp_cpy [r0 + GUI_CTX_X], r5
			vp_cpy [r0 + GUI_CTX_Y], r6
			vp_add [r1 + GUI_VIEW_X], r5
			vp_add [r1 + GUI_VIEW_Y], r6
			vp_cpy r5, [r0 + GUI_CTX_X]
			vp_cpy r6, [r0 + GUI_CTX_Y]
			vp_cpy r1, [r4 + DRAW_VIEW_VIEW]
			vp_cpy r2, [r4 + DRAW_VIEW_NEXT]
			vp_call draw_view
			vp_cpy [r4 + DRAW_VIEW_CTX], r0
			vp_cpy [r4 + DRAW_VIEW_VIEW], r1
			vp_cpy [r4 + DRAW_VIEW_NEXT], r2
			vp_cpy [r0 + GUI_CTX_X], r5
			vp_cpy [r0 + GUI_CTX_Y], r6
			vp_sub [r1 + GUI_VIEW_X], r5
			vp_sub [r1 + GUI_VIEW_Y], r6
			vp_cpy r5, [r0 + GUI_CTX_X]
			vp_cpy r6, [r0 + GUI_CTX_Y]
		loop_end
		vp_add DRAW_VIEW_SIZE, r4
		vp_ret

	fn_function_end
