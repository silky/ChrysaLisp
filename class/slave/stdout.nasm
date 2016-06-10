%include 'inc/func.inc'
%include 'class/class_slave.inc'
%include 'class/class_master.inc'

	fn_function class/slave/stdout
		;inputs
		;r0 = slave object
		;r1 = buffer
		;r2 = length
		;trashes
		;all but r0, r4

		ptr inst
		ptr buffer
		ulong length
		ptr msg

		push_scope
		retire {r0, r1, r2}, {inst, buffer, length}

		static_call sys_mail, alloc_parcel, {slave_mail_stream_size + length}, {msg}
		assign {inst->slave_stdout_id.id_mbox}, {msg->msg_dest.id_mbox}
		assign {inst->slave_stdout_id.id_cpu}, {msg->msg_dest.id_cpu}
		static_call sys_mem, copy, {buffer, &msg->slave_mail_stream_data, length}, {_, _}
		assign {inst->slave_stdout_seqnum}, {msg->slave_mail_stream_seqnum}
		assign {master_state_started}, {msg->slave_mail_stream_state}
		static_call sys_mail, send, {msg}

		assign {inst->slave_stdout_seqnum + 1}, {inst->slave_stdout_seqnum}

		eval {inst}, r0
		pop_scope
		vp_ret

	fn_function_end