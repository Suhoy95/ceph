// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2013 CohortFS, LLC
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */

#ifndef SIMPLEDISPATCHER_H_
#define SIMPLEDISPATCHER_H_

#include "msg/Dispatcher.h"
#include "msg/Messenger.h"

class SimpleDispatcher: public Dispatcher {
private:
  bool active;
  Messenger *messenger;
  uint64_t dcount;
public:
  explicit SimpleDispatcher(Messenger *msgr);
  ~SimpleDispatcher() override;

  uint64_t get_dcount() { return dcount; }

  void set_active() {
    active = true;
  };

  // how i receive messages
  bool ms_dispatch(Message *m) override;

  /**
   * This function will be called whenever a new Connection is made to the
   * Messenger.
   *
   * @param con The new Connection which has been established. You are not
   * granted a reference to it -- take one if you need one!
   */
  void ms_handle_connect(Connection *con) override { };

  /**
   * Callback indicating we have accepted an incoming connection.
   *
   * @param con The (new or existing) Connection associated with the session
   */
  void ms_handle_accept(Connection *con) override { };

  /*
   * this indicates that the ordered+reliable delivery semantics have
   * been violated.  Messages may have been lost due to a fault
   * in the network connection.
   * Only called on lossy Connections or those you've
   * designated mark_down_on_empty().
   *
   * @param con The Connection which broke. You are not granted
   * a reference to it.
   */
  bool ms_handle_reset(Connection *con) override;

  /**
   * This indicates that the ordered+reliable delivery semantics
   * have been violated because the remote somehow reset.
   * It implies that incoming messages were dropped, and
   * probably some of our previous outgoing messages were too.
   *
   * @param con The Connection which broke. You are not granted
   * a reference to it.
   */
  void ms_handle_remote_reset(Connection *con) override;
  
  bool ms_handle_refused(Connection *con) override { return false; }

  /**
   * @defgroup Authentication
   * @{
   */
  /**
   * Retrieve the AuthAuthorizer for the given peer type. It might not
   * provide one if it knows there is no AuthAuthorizer for that type.
   *
   * @param dest_type The peer type we want the authorizer for.
   * @param a Double pointer to an AuthAuthorizer. The Dispatcher will fill
   * in *a with the correct AuthAuthorizer, if it can. Make sure that you have
   * set *a to NULL before calling in.
   *
   * @return True if this function call properly filled in *a, false otherwise.
   */
  bool ms_get_authorizer(int dest_type, AuthAuthorizer **a) override {
    return false;
  };

  int ms_handle_authentication(Connection *con) override {
    return 1;
  }
};

#endif /* SIMPLEDISPATCHER_H_ */
