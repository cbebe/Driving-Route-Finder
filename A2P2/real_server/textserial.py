'''
Provides the TextSerial class to add a text-based interface for
serial.Serial based on TextIOWrapper.

Created on Jan 11, 2015
@author: csaba
'''

import io
import serial
import sys


class TextSerial(io.TextIOWrapper):
    '''
    Adds text-based interface for serial.Serial.

    The purpose of this class is to simplify text-based interaction
    with the serial port. The usual text based services (write a string,
    read a line, read strings, iterate through the inputs using a for
    loop) are made available via using this class.
    '''

    def __init__(self, *args, **kwargs):
        '''Constructs a TextSerial object around two instances of
        serial.Serial.

        Args:
            All arguments, except encoding, line_buffering and ser are passed
            to the constructors of serial.Serial.

        Keyword only arguments:
            timeout (float): Timeout for reading operations, defaults to 0,
                non-blocking mode. None is indefinite blocking.
                Unit is seconds.
            encoding (str): The text encoding to be used. Defaults to 'ascii'.
            errors (str): How encoding and decoding errors should be handled.
                For details see the documentation of TextIOWrapper.
                Defaults to None.
            newline (str, or None): Controls how line endings are handled.
                For details see the documentation of TextIOWrapper.
                Defaults to None.
            line_buffering (bool): Whether upon seeing '\n' in the output,
                the stream will be flushed.
                If this is set to False, it is the user's responsibility to
                call flush to make sure that the data is actually sent to the
                receiver. Defaults to True.
            write_through (bool): if True, calls to write() are guaranteed not
                to be buffered. Defaults to False. Only in Python 3.3 or newer.
            ser (Serial): The serial object to be used,
                for both input and output. This will work properly
                only with some serial objects, such as the loop back object.
                This is meant mainly for testing purposes.

        '''
        # We initialize two Serial objects; one for the input, another
        # for the output. We need two objects, as upon the destruction
        # of this object, BufferedRWPair will attempt to close both
        # the reader and the writer, leading to an exception.
        #
        # The documentation of BufferedRWPair at
        # https://docs.python.org/3/library/io.html#io.BufferedRWPair
        # mentions that one should not pass the same object to it both
        # as a reader and writer, but the suggestion there to use
        # BufferedRandom won't work for us as our stream (serial)
        # is non-seekable. Hence, we are forced to open the serial port
        # twice.
        #
        def getkwarg(parname, defval, kwargs):
            v = defval
            if parname in kwargs:
                v = kwargs[parname]
                del kwargs[parname]
            return v

        # get and remove TextIOWrapper specific arguments;
        # luckily for us these do not overlap with any of the serial arguments
        encoding = getkwarg('encoding', 'ascii', kwargs)
        errors = getkwarg('errors', None, kwargs)
        newline = getkwarg('newline', None, kwargs)
        line_buffering = getkwarg('line_buffering', True, kwargs)
        write_through = getkwarg('write_through', False, kwargs)

        # get timeout; not used
        # timeout = kwargs.get('timeout', 0)

        if 'ser' in kwargs:
            self.ser_in = self.ser_out = kwargs.get('ser')
        else:
            self.ser_in = serial.Serial(*args, **kwargs)
            self.ser_out = serial.Serial(*args, **kwargs)

        # note: a try/catch won't work here, as a failing __init__
        # is kinda fatal, it will put the object into a failed
        # state and I don't know how to recover it from that state
        # otherwise I would prefer try/catch

        # note 2: We need to set BufferedRWPair's buffer size to 1;
        # BufferedRWPair forwards the read call to BuferredReader's
        # read function, which expects the
        # underlying stream to return None or b"" when there
        # is no more data available, rather than to block.
        # However, as of pyserial 2.7, Serial.read() blocks.
        # What would be needed is a non-blocking version of Serial.
        # This can be achieved by setting the timeout to zero on Serial,
        # but this kinda defeats the purpose of having nonzero timeouts.
        # Hence, we turn off buffering.
        if sys.version_info.major >= 3 and sys.version_info.minor >= 3:
            # This works in Python 3.3 and newer
            super().__init__(io.BufferedRWPair(
                self.ser_in, self.ser_out, 1),
                encoding=encoding, errors=errors, newline=newline,
                line_buffering=line_buffering, write_through=write_through)
        else:
            # no write_through in earlier pythons
            super().__init__(io.BufferedRWPair(
                self.ser_in, self.ser_out, 1),
                encoding=encoding, errors=errors, newline=newline,
                line_buffering=line_buffering)

        # Explanation of the next line:
        # TextIOWrapper reads data in chunks, and NOT ONLY THROUGH
        # BufferedRWPair's buffering interface, but it has its own chunk-based
        # processing. When the sender did not send data with size>=chunk size,
        # processing will block indefinitely (or as long as the timeout is).
        # The solution suggested here is to reset the chunk size to 1.
        # The cons is that maybe data processing will not be the most
        # efficient, as data is obtained one byte at time.
        # Adding a timeout kind-of defeats the purpose of timeouts;
        # without changing the chunk size, the wrapper will always wait
        # until the timeout expires! This totally defeats the purpose of
        # timeouts (again).
        self._CHUNK_SIZE = 1

    def setTimeout(self, timeout):
        '''Sets the timeout for reading'''
        self.ser_in.setTimeout(timeout)

    def getTimeout(self):
        '''Gets the timeout for reading'''
        return self.ser_in.setTimeout()


def __main():
    '''Tests the interface'''

    print("Using a loopback simulator:")
    with TextSerial(ser=serial.serial_for_url('loop://', timeout=0)) as ser:
        # send 'hello'
        print("hello", file=ser)
        # as this is a loop back, we receive hello:
        line = ser.readline()
        # an empty line sent back has actually its new line character
        print("Received:", line.rstrip('\r\n'))
        # demonstrate a for loop:
        lno = 0
        print(lno, file=ser)
        for line in ser:
            print("Received:", line.rstrip('\r\n'))
            if lno >= 10:
                break
            lno += 1
            print(lno, file=ser)
    try:
        # if one has an echo program:
        port = ('/dev/tty.usbmodem1411'
                if sys.platform == 'darwin' else
                '/dev/ttyACM0')
        baud = 9600
        print("Attempting to use an 'echo' program on", port)
        # we use a single newline character when communicating with the
        # Arduino to mark the end of lines, hence we need to set the
        # newline parameter to '\n':

        with TextSerial(port, baud, newline='\n') as ser:
            for i in range(50):
                msg = 'hello'
                print("Sending:", msg)
                print(msg, file=ser)
                line = ser.readline()
                print("Got:", line.rstrip('\n'))
        print("Closed serial port.")
        timeout = None
        print("Re-running the test with a timeout of %s seconds" % timeout)
        print("First.. waiting 5 seconds..")
        import time
        for i in range(5):
            time.sleep(1)
            print('.', end='')  # this won't flush
            sys.stdout.flush()
        print()
        print("Starting..")
        time.sleep(1)
        with TextSerial(port, baud, newline='\n', timeout=timeout) as ser:
            for i in range(50):
                msg = 'hello'
                print("Sending:", msg)
                print(msg, file=ser)
                line = ser.readline()
                print("Got:", line.rstrip('\n'))

    except BaseException as exc:
        print(exc)

    print("bye!")


if __name__ == "__main__":
    __main()
