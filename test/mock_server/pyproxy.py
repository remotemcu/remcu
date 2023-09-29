#!/usr/bin/env python

__license__     = 'MIT'
__version__     = '0.1'


"""
PLAY/DUMP TCP proxy.
"""

import argparse
import signal
import logging
import select
import socket, time


FORMAT = '%(asctime)-15s %(levelname)-10s %(message)s'
logging.basicConfig(format=FORMAT)
LOGGER = logging.getLogger()

BUFFER_SIZE = 2 ** 10  # 1024. Keep buffer size as power of 2.

    
def tcp_dump_proxy(src, dst, file):
    """Run TCP dump proxy.
    
    Arguments:
    src -- Source IP address and port string. I.e.: '127.0.0.1:8000'
    dst -- Destination IP address and port. I.e.: '127.0.0.1:8888'
    """
    LOGGER.debug('Starting TCP dump proxy...')
    LOGGER.debug('Src: {}'.format(src))
    LOGGER.debug('Dst: {}'.format(dst))
    
    sockets = []
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    ip = ip_to_tuple(src)
    # print(ip)
    s.bind(ip)
    s.listen(1)

    s_src, _ = s.accept()

    s_dst = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s_dst.connect(ip_to_tuple(dst)) 
    
    sockets.append(s_src)
    sockets.append(s_dst)

    with open(file, "w") as f:

        def dump_data_handler(direct, data):
            if data != b'':
                print(direct, data)
                f.write(f"{direct}:{data.hex()}\n")

            return data

        while True:
            s_read, _, _ = select.select(sockets, [], [])
            
            for s in s_read:
                data = s.recv(BUFFER_SIZE)
            
                if s == s_src:
                    d = dump_data_handler('client', data)
                    s_dst.sendall(d)
                elif s == s_dst:
                    d = dump_data_handler('server', data)
                    s_src.sendall(d)
# end-of-function tcp_proxy    


def play_dump(src, file):

    LOGGER.debug('Starting TCP play...')
    LOGGER.debug('Src: {}'.format(src))
    
    sockets = []
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(ip_to_tuple(src))
    s.listen(1)
    s_src, _ = s.accept()
    with open(file, "r") as f:
        lines = f.readlines()


    i = 0
    while True:
        s_read, _, _ = select.select([s_src], [], [])
        
        
        if len(s_read) == 0:
            time.sleep(1)
            continue
        ss = s_read[0]
        data = ss.recv(BUFFER_SIZE)
        if data == b'':
            continue
        print("got:", data)


        if i >= len(lines):
            print("test pass")
            break

        mess = lines[i]

        _CLIENT = "client:"
        _SERVER = "server:"

        if mess.startswith(_CLIENT):
            mess = mess[len(_CLIENT):]
            bmess = bytes.fromhex(mess)
            print(_CLIENT, bmess)
            if bmess != data:
                print(f"no equal {len(bmess)}/{len(data)} : {bmess}, {data}")
                exit(-1)
        else:
            print("no start client message")
            exit(-3)

        i += 1
        if i >= len(lines):
            print("test pass")
            exit(0)
        if lines[i].startswith(_CLIENT):
            continue

        while lines[i].startswith(_SERVER):
            mess = lines[i]
            print("message from file:", mess)
            resp = mess[len(_SERVER):]
            bresp = bytes.fromhex(resp)
            print(_SERVER, bresp)
            s_src.sendall(bresp)
            i += 1
            if i >= len(lines):
                print("test pass")
                exit(0)
  


def ip_to_tuple(ip):
    """Parse IP string and return (ip, port) tuple.
    
    Arguments:
    ip -- IP address:port string. I.e.: '127.0.0.1:8000'.
    """
    ip, port = ip.split(':')
    return (ip, int(port))
# end-of-function ip_to_tuple


def main():
    """Main method."""
    parser = argparse.ArgumentParser(description='TCP/UPD proxy.')
    
    # play dump groups
    proto_group = parser.add_mutually_exclusive_group(required=True)
    proto_group.add_argument('--dump', action='store_true', help='TCP dump proxy')
    proto_group.add_argument('--play', action='store_true', help='TCP dump play')
    
    parser.add_argument('-s', '--src', required=True, help='Source IP and port, i.e.: 127.0.0.1:8000')
    parser.add_argument('-d', '--dst', help='Destination IP and port, i.e.: 127.0.0.1:8888')
    parser.add_argument('-f', '--file', required=True, help='dump file for play')
    
    output_group = parser.add_mutually_exclusive_group()
    output_group.add_argument('-q', '--quiet', action='store_true', help='Be quiet')
    output_group.add_argument('-v', '--verbose', action='store_true', help='Be loud')
    
    args = parser.parse_args()
    
    if args.quiet:
        LOGGER.setLevel(logging.CRITICAL)
    if args.verbose:
        LOGGER.setLevel(logging.NOTSET)
    
    if args.dump:
        tcp_dump_proxy(args.src, args.dst, args.file)
    elif args.play:
        play_dump(args.src, args.file)
# end-of-function main    


if __name__ == '__main__':
    main()