/*
 * This file is part of the SSH Library
 *
 * Copyright (c) 2009 by Aris Adamantiadis
 *
 * The SSH Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * The SSH Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the SSH Library; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef CHANNELS_H_
#define CHANNELS_H_
#include "libssh/priv.h"

/**  @internal
 * Describes the different possible states in a
 * outgoing (client) channel request
 */
enum ssh_channel_request_state_e {
	/** No request has been made */
	SSH_CHANNEL_REQ_STATE_NONE = 0,
	/** A request has been made and answer is pending */
	SSH_CHANNEL_REQ_STATE_PENDING,
	/** A request has been replied and accepted */
	SSH_CHANNEL_REQ_STATE_ACCEPTED,
	/** A request has been replied and refused */
	SSH_CHANNEL_REQ_STATE_DENIED,
	/** A request has been replied and an error happend */
	SSH_CHANNEL_REQ_STATE_ERROR
};

struct ssh_channel_struct {
    struct ssh_channel_struct *prev;
    struct ssh_channel_struct *next;
    ssh_session session; /* SSH_SESSION pointer */
    uint32_t local_channel;
    uint32_t local_window;
    int local_eof;
    uint32_t local_maxpacket;

    uint32_t remote_channel;
    uint32_t remote_window;
    int remote_eof; /* end of file received */
    uint32_t remote_maxpacket;
    int open; /* shows if the channel is still opened */
    int delayed_close;
    ssh_buffer stdout_buffer;
    ssh_buffer stderr_buffer;
    void *userarg;
    int version;
    int blocking;
    int exit_status;
    enum ssh_channel_request_state_e request_state;
};

SSH_PACKET_CALLBACK(ssh_packet_channel_open_conf);
SSH_PACKET_CALLBACK(ssh_packet_channel_open_fail);
SSH_PACKET_CALLBACK(ssh_packet_channel_success);
SSH_PACKET_CALLBACK(ssh_packet_channel_failure);

ssh_channel channel_new(ssh_session session);
int channel_default_bufferize(ssh_channel channel, void *data, int len,
        int is_stderr);
uint32_t ssh_channel_new_id(ssh_session session);
ssh_channel ssh_channel_from_local(ssh_session session, uint32_t id);
int channel_write_common(ssh_channel channel, const void *data,
    uint32_t len, int is_stderr);

#endif /* CHANNELS_H_ */
