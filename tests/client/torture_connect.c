/*
 * This file is part of the SSH Library
 *
 * Copyright (c) 2010 by Aris Adamantiadis
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

#define LIBSSH_STATIC

#include "torture.h"
#include <libssh/libssh.h>

static void setup(void **state) {
    int verbosity=torture_libssh_verbosity();
    ssh_session session = ssh_new();

    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

    *state = session;
}

static void teardown(void **state) {
    ssh_session session = *state;
    ssh_disconnect(session);
    ssh_free(session);
}

static void torture_connect_nonblocking(void **state) {
    ssh_session session = *state;

    int rc;

    rc = ssh_options_set(session, SSH_OPTIONS_HOST, "localhost");
    assert_true(rc == SSH_OK);
    ssh_set_blocking(session,0);

    do {
    	rc = ssh_connect(session);
    	assert_true(rc != SSH_ERROR);
    } while(rc == SSH_AGAIN);

    assert_true(rc==SSH_OK);

}

static void torture_connect_double(void **state) {
    ssh_session session = *state;

    int rc;

    rc = ssh_options_set(session, SSH_OPTIONS_HOST, "localhost");
    assert_true(rc == SSH_OK);
    rc = ssh_connect(session);
    assert_true(rc == SSH_OK);
    ssh_disconnect(session);

    rc = ssh_connect(session);
    assert_true(rc == SSH_OK);

}

static void torture_connect_failure(void **state){
    /*
     * The intent of this test is to check that a fresh
     * ssh_new/ssh_disconnect/ssh_free sequence doesn't crash/leak
     * and the behavior of a double ssh_disconnect
     */
    ssh_session session = *state;
    ssh_disconnect(session);
}
int torture_run_tests(void) {
    int rc;
    const UnitTest tests[] = {
        unit_test_setup_teardown(torture_connect_nonblocking, setup, teardown),
        unit_test_setup_teardown(torture_connect_double, setup, teardown),
        unit_test_setup_teardown(torture_connect_failure, setup, teardown),
    };

    ssh_init();

    rc = run_tests(tests);

    ssh_finalize();
    return rc;
}
