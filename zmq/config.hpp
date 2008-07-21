/*
    Copyright (c) 2007-2008 FastMQ Inc.

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ZMQ_CONFIG_HPP_INCLUDED__
#define __ZMQ_CONFIG_HPP_INCLUDED__

namespace zmq
{

    //  Compile-time settings for 0MQ
    enum
    {
        //  CPU frequency in MHz - the number doesn't have to be precise -
        //  incorrectly set CPU frequency will result in different timeouts
        //  at various places in the code, however, none of them is critical
        //  unless the discrepancy is in whole orders of magnitude
        cpu_frequency = 3000,

        //  Maximal batching size for incoming backend protocol messages.
        //  So, if there are 10 messages that fit into the batch size, all of
        //  them may be read by a single 'read' system call, thus avoiding
        //  unnecessary network stack traversals.
        bp_in_batch_size = 8192,

        //  Maximal batching size for outgoing backend protocol messages.
        //  So, if there are 10 messages that fit into the batch size, all of
        //  them may be sent by a single 'write' system call, thus avoiding
        //  unnecessary network stack traversals.
        bp_out_batch_size = 8192,

        //  Number of new messages in message pipe which triggers new memory
        //  allocation.
        message_pipe_granularity = 256,

        //  Number of new commands in command pipe which triggers new memory
        //  allocation.
        command_pipe_granularity = 16,

        //  Maximal size of "Very Small Message". VSMs are passed by value
        //  to avoid excessive allocation/deallocation.
        max_vsm_size = 62,

        //  Maximal time (in microseconds) that a command can be delayed
        max_command_delay = 1000
    };

}

#endif