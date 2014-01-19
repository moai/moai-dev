/*
 * Copyright (c) 2013, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE transport_iostream_connection
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <cstring>
#include <string>

#include <websocketpp/common/memory.hpp>

#include <websocketpp/error.hpp>
#include <websocketpp/transport/iostream/connection.hpp>

// Policies
#include <websocketpp/concurrency/basic.hpp>
#include <websocketpp/logger/basic.hpp>

struct config {
    typedef websocketpp::concurrency::basic concurrency_type;
    typedef websocketpp::log::basic<concurrency_type,
        websocketpp::log::elevel> elog_type;
    typedef websocketpp::log::basic<concurrency_type,
        websocketpp::log::alevel> alog_type;
};

typedef websocketpp::transport::iostream::connection<config> iostream_con;

using websocketpp::transport::iostream::error::make_error_code;

struct stub_con : public iostream_con {
    typedef stub_con type;
    typedef websocketpp::lib::shared_ptr<type> ptr;
    typedef iostream_con::timer_ptr timer_ptr;

    stub_con(bool is_server, config::alog_type &a, config::elog_type & e)
        : iostream_con(is_server,a,e)
        // Set the error to a known code that is unused by the library
        // This way we can easily confirm that the handler was run at all.
        , ec(websocketpp::error::make_error_code(websocketpp::error::test))
    {}

    /// Get a shared pointer to this component
    ptr get_shared() {
        return websocketpp::lib::static_pointer_cast<type>(iostream_con::get_shared());
    }

    void write(std::string msg) {
        iostream_con::async_write(
            msg.data(),
            msg.size(),
            websocketpp::lib::bind(
                &stub_con::handle_op,
                type::get_shared(),
                websocketpp::lib::placeholders::_1
            )
        );
    }

    void write(std::vector<websocketpp::transport::buffer> & bufs) {
        iostream_con::async_write(
            bufs,
            websocketpp::lib::bind(
                &stub_con::handle_op,
                type::get_shared(),
                websocketpp::lib::placeholders::_1
            )
        );
    }

    void async_read_at_least(size_t num_bytes, char *buf, size_t len)
	{
        iostream_con::async_read_at_least(
            num_bytes,
            buf,
            len,
            websocketpp::lib::bind(
                &stub_con::handle_op,
                type::get_shared(),
                websocketpp::lib::placeholders::_1
            )
        );
    }

    void handle_op(const websocketpp::lib::error_code& e) {
        ec = e;
    }

    websocketpp::lib::error_code ec;
};

// Stubs
config::alog_type a;
config::elog_type e;

BOOST_AUTO_TEST_CASE( const_methods ) {
    iostream_con::ptr con(new iostream_con(true,a,e));

    BOOST_CHECK( con->is_secure() == false );
    BOOST_CHECK( con->get_remote_endpoint() == "iostream transport" );
}

BOOST_AUTO_TEST_CASE( write_before_ostream_set ) {
    stub_con::ptr con(new stub_con(true,a,e));

    con->write("foo");
    BOOST_CHECK( con->ec == make_error_code(websocketpp::transport::iostream::error::output_stream_required) );

    std::vector<websocketpp::transport::buffer> bufs;
    con->write(bufs);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::transport::iostream::error::output_stream_required) );
}

BOOST_AUTO_TEST_CASE( async_write ) {
    stub_con::ptr con(new stub_con(true,a,e));

    std::stringstream output;

    con->register_ostream(&output);

    con->write("foo");

    BOOST_CHECK( !con->ec );
    BOOST_CHECK( output.str() == "foo" );
}

BOOST_AUTO_TEST_CASE( async_write_vector_0 ) {
    std::stringstream output;

    stub_con::ptr con(new stub_con(true,a,e));
    con->register_ostream(&output);

    std::vector<websocketpp::transport::buffer> bufs;

    con->write(bufs);

    BOOST_CHECK( !con->ec );
    BOOST_CHECK( output.str() == "" );
}

BOOST_AUTO_TEST_CASE( async_write_vector_1 ) {
    std::stringstream output;

    stub_con::ptr con(new stub_con(true,a,e));
    con->register_ostream(&output);

    std::vector<websocketpp::transport::buffer> bufs;

    std::string foo = "foo";

    bufs.push_back(websocketpp::transport::buffer(foo.data(),foo.size()));

    con->write(bufs);

    BOOST_CHECK( !con->ec );
    BOOST_CHECK( output.str() == "foo" );
}

BOOST_AUTO_TEST_CASE( async_write_vector_2 ) {
    std::stringstream output;

    stub_con::ptr con(new stub_con(true,a,e));
    con->register_ostream(&output);

    std::vector<websocketpp::transport::buffer> bufs;

    std::string foo = "foo";
    std::string bar = "bar";

    bufs.push_back(websocketpp::transport::buffer(foo.data(),foo.size()));
    bufs.push_back(websocketpp::transport::buffer(bar.data(),bar.size()));

    con->write(bufs);

    BOOST_CHECK( !con->ec );
    BOOST_CHECK( output.str() == "foobar" );
}

BOOST_AUTO_TEST_CASE( async_read_at_least_too_much ) {
    stub_con::ptr con(new stub_con(true,a,e));

    char buf[10];

    con->async_read_at_least(11,buf,10);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::transport::iostream::error::invalid_num_bytes) );
}

BOOST_AUTO_TEST_CASE( async_read_at_least_double_read ) {
    stub_con::ptr con(new stub_con(true,a,e));

    char buf[10];

    con->async_read_at_least(5,buf,10);
    con->async_read_at_least(5,buf,10);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::transport::iostream::error::double_read) );
}

BOOST_AUTO_TEST_CASE( async_read_at_least ) {
    stub_con::ptr con(new stub_con(true,a,e));

    char buf[10];

    memset(buf,'x',10);

    con->async_read_at_least(5,buf,10);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::error::test) );

    std::stringstream channel;
	channel << "abcd";
	channel >> *con;
	BOOST_CHECK( channel.tellg() == -1 );
    BOOST_CHECK( con->ec == make_error_code(websocketpp::error::test) );

    std::stringstream channel2;
	channel2 << "e";
	channel2 >> *con;
	BOOST_CHECK( channel2.tellg() == -1 );
    BOOST_CHECK( !con->ec );
    BOOST_CHECK( std::string(buf,10) == "abcdexxxxx" );

    std::stringstream channel3;
	channel3 << "f";
	channel3 >> *con;
	BOOST_CHECK( channel3.tellg() == 0 );
    BOOST_CHECK( !con->ec );
    BOOST_CHECK( std::string(buf,10) == "abcdexxxxx" );
    con->async_read_at_least(1,buf+5,5);
    channel3 >> *con;
    BOOST_CHECK( channel3.tellg() == -1 );
    BOOST_CHECK( !con->ec );
    BOOST_CHECK( std::string(buf,10) == "abcdefxxxx" );
}

BOOST_AUTO_TEST_CASE( async_read_at_least2 ) {
    stub_con::ptr con(new stub_con(true,a,e));

    char buf[10];

    memset(buf,'x',10);

    con->async_read_at_least(5,buf,5);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::error::test) );

    std::stringstream channel;
	channel << "abcdefg";
	channel >> *con;
    BOOST_CHECK( channel.tellg() == 5 );
    BOOST_CHECK( !con->ec );
    BOOST_CHECK( std::string(buf,10) == "abcdexxxxx" );

    con->async_read_at_least(1,buf+5,5);
	channel >> *con;
	BOOST_CHECK( channel.tellg() == -1 );
    BOOST_CHECK( !con->ec );
    BOOST_CHECK( std::string(buf,10) == "abcdefgxxx" );
}

void timer_callback_stub(const websocketpp::lib::error_code & ec) {}

BOOST_AUTO_TEST_CASE( set_timer ) {
   stub_con::ptr con(new stub_con(true,a,e));

    stub_con::timer_ptr tp = con->set_timer(1000,timer_callback_stub);

    BOOST_CHECK( !tp );
}

BOOST_AUTO_TEST_CASE( async_read_at_least_read_some ) {
    stub_con::ptr con(new stub_con(true,a,e));

    char buf[10];
    memset(buf,'x',10);

    con->async_read_at_least(5,buf,5);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::error::test) );

    char input[10] = "abcdefg";
    BOOST_CHECK_EQUAL(con->read_some(input,5), 5);
    BOOST_CHECK( !con->ec );
    BOOST_CHECK_EQUAL( std::string(buf,10), "abcdexxxxx" );

    BOOST_CHECK_EQUAL(con->read_some(input+5,2), 0);
    BOOST_CHECK( !con->ec );
    BOOST_CHECK_EQUAL( std::string(buf,10), "abcdexxxxx" );

    con->async_read_at_least(1,buf+5,5);
    BOOST_CHECK_EQUAL(con->read_some(input+5,2), 2);
    BOOST_CHECK( !con->ec );
    BOOST_CHECK_EQUAL( std::string(buf,10), "abcdefgxxx" );
}

BOOST_AUTO_TEST_CASE( eof_flag ) {
    stub_con::ptr con(new stub_con(true,a,e));
    char buf[10];
    con->async_read_at_least(5,buf,5);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::error::test) );
    con->eof();
    BOOST_CHECK_EQUAL( con->ec, make_error_code(websocketpp::transport::error::eof) );
}

BOOST_AUTO_TEST_CASE( fatal_error_flag ) {
    stub_con::ptr con(new stub_con(true,a,e));
    char buf[10];
    con->async_read_at_least(5,buf,5);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::error::test) );
    con->fatal_error();
    BOOST_CHECK_EQUAL( con->ec, make_error_code(websocketpp::transport::error::pass_through) );
}

BOOST_AUTO_TEST_CASE( shared_pointer_memory_cleanup ) {
    stub_con::ptr con(new stub_con(true,a,e));

    BOOST_CHECK_EQUAL(con.use_count(), 1);

    char buf[10];
    memset(buf,'x',10);
    con->async_read_at_least(5,buf,5);
    BOOST_CHECK( con->ec == make_error_code(websocketpp::error::test) );
    BOOST_CHECK_EQUAL(con.use_count(), 2);

    char input[10] = "foo";
    con->read_some(input,3);
    BOOST_CHECK_EQUAL(con.use_count(), 2);

    con->read_some(input,2);
    BOOST_CHECK_EQUAL( std::string(buf,10), "foofoxxxxx" );
    BOOST_CHECK_EQUAL(con.use_count(), 1);

    con->async_read_at_least(5,buf,5);
    BOOST_CHECK_EQUAL(con.use_count(), 2);

    con->eof();
    BOOST_CHECK_EQUAL( con->ec, make_error_code(websocketpp::transport::error::eof) );
    BOOST_CHECK_EQUAL(con.use_count(), 1);
}

