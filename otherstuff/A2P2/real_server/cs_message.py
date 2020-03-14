import sys

"""
client-server messaging facility

This module provides the ability to send to and receive messages from a
client attached to a serial port. A message is a string of ascii
characters terminated with a new-line, as understood by the TextSerial
interface.

It also provides a diagnostic message tunnel capability.  Any message
coming from the client that begins with "D" is intercepted and printed
on stderr (asuming logging in on).

Combined with the dprintf library for the arduino, this enables the
client code to supply diagnostic information as if it was connected to
it's own stderr.

"""

# when True this generates output to stderr, when False does not.
# modify with set_loggin, query with get logging
logging = True


def set_logging(new):
    """
    Set logging on True, or off False.
    Return the previous value of the logging state.
    """

    global logging
    old = logging
    logging = new
    return old


def get_logging():
    global logging
    return logging


def escape_nl(msg):
    """
    It's nice to know if we actually sent a complete line ending in
    \n, so escape it for display.
    """
    if msg != '' and msg[-1] == "\n":
        return msg[:-1] + "\\n"
    return msg


def log_msg(msg):
    """
    If logging is on, send this message to stderr, making sure to
    show if there was a new line.  The message is prefixed with "L ", to
    indicate that it was a logging message, not a message received from
    the client.

    Flush the output buffer so we don't lose any of the message.
    """
    global logging
    if logging:
        print("L |{}|".format(escape_nl(msg)), file=sys.stderr, flush=True)


def send_msg_to_client(channel, msg):
    """
    Send a message to the client over channel, and log it if
    logging on.  msg should not end with a new line, as one will be
    added by the print.  Flush to ensure complete line is sent.
    """

    print(msg, file=channel, flush=True)
    if logging:
        log_msg(msg)


def receive_msg_from_client(channel):
    """
    Wait for a message from the client.  If a diagnostic 'D' type
    message comes in, intercept it, print it on stderr,  and wait
    for a proper one to arrive.

    The message is returned unchanged, terminating new line included.
    """
    global logging

    while True:
        msg = next(channel)

        # If message begins with a D, then its a diagnostic message
        # from the client, and should be sent to stderr and ignored.

        if msg.strip()[:1] == "D":
            if logging:
                print(escape_nl(msg), file=sys.stderr, flush=True)
            continue
        else:
            break

    return msg
