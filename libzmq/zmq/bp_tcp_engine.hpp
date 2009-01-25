/*
    Copyright (c) 2007-2009 FastMQ Inc.

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __ZMQ_BP_TCP_ENGINE_HPP_INCLUDED__
#define __ZMQ_BP_TCP_ENGINE_HPP_INCLUDED__

#include <string>

#include <zmq/stdint.hpp>
#include <zmq/export.hpp>
#include <zmq/i_pollable.hpp>
#include <zmq/i_thread.hpp>
#include <zmq/engine_base.hpp>
#include <zmq/bp_encoder.hpp>
#include <zmq/bp_decoder.hpp>
#include <zmq/tcp_socket.hpp>
#include <zmq/tcp_listener.hpp>

namespace zmq
{

    //  BP/TCP engine is defined by follwowing properties:
    //
    //  1. Underlying transport is TCP.
    //  2. Wire-level protocol is 0MQ backend protocol.
    //  3. Communicates with I/O thread via file descriptors.

    class bp_tcp_engine_t :
        public engine_base_t <true,true>,
        public i_pollable
    {
        //  Allow class factory to create this engine.
        friend class engine_factory_t;

        //  Allow BP/TCP listener to create the engine.
        friend class bp_tcp_listener_t;

    public:

        //  i_engine interface implementation.
        i_pollable *cast_to_pollable ();
        void get_watermarks (uint64_t *hwm_, uint64_t *lwm_);
        void revive (pipe_t *pipe_);
        void head (pipe_t *pipe_, uint64_t position_);
        void send_to (pipe_t *pipe_);
        void receive_from (pipe_t *pipe_);

        //  i_pollable interface implementation.
        void register_event (i_poller *poller_);
        void in_event ();
        void out_event ();
        void unregister_event ();

    private:

        enum engine_state_t {

            //  TCP connection has not been established yet. The engine
            //  does not perform any I/O operations.
            engine_connecting,

            //  Engine is fully operational.
            engine_connected,

            //  Engine is already shutting down, waiting for confirmation
            //  from other threads.
            engine_shutting_down
        };

        //  Creates bp_tcp_engine. Underlying TCP connection is initialised
        //  using hostname parameter. Local object name is simply stored
        //  and passed to error handler function when connection breaks.
        bp_tcp_engine_t (i_thread *calling_thread_, i_thread *thread_,
            const char *hostname_, const char *local_object_,
            const char *arguments_);
        bp_tcp_engine_t (i_thread *calling_thread_, i_thread *thread_,
            tcp_listener_t &listener_, const char *local_object_);

        ~bp_tcp_engine_t ();

        //  Reconnect to the remote peer.
        void reconnect ();

        //  Initialise engine shutdown.
        void shutdown ();

        //  Buffer to be written to the underlying socket.
        unsigned char *writebuf;
        int writebuf_size;
        int write_size;
        int write_pos;

        //  Buffer to read from undrlying socket.
        unsigned char *readbuf;
        int readbuf_size;
        int read_size;
        int read_pos;

        //  Backend wire-level protocol encoder.
        bp_encoder_t encoder;

        //  Backend wire-level protocol decoder.
        bp_decoder_t decoder;

        //  Underlying TCP/IP socket.
        tcp_socket_t *socket;

        //  Callback to poller.
        i_poller *poller;

        //  Poll handle associated with this engine.
        handle_t handle;

        //  Name of the object on this side of the connection (exchange/queue).
        std::string local_object;

        //  Flag indicating whether the engine should try to reconnect on
        //  connection failure or not.
        bool reconnect_flag;

        //  The hostname the engine should reconnect to. Undefined when
        //  reconnect_flag is false.
        std::string hostname;

        //  Engine state.
        engine_state_t state;

        bp_tcp_engine_t (const bp_tcp_engine_t&);
        void operator = (const bp_tcp_engine_t&);
    };

}

#endif