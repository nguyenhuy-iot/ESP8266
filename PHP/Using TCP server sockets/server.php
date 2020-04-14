<?php
$conn = stream_socket_server('tcp://192.168.1.14:4444');
while ($socket = stream_socket_accept($conn)) {
    $pkt = stream_socket_recvfrom($socket, 1500, 0, $peer);
    if (false === empty($pkt)) {
        stream_socket_sendto($socket, 'Received pkt ' . $pkt, 0, $peer);
    }
    fclose($socket);
    usleep(10000); //100ms delay
}
stream_socket_shutdown($conn, \STREAM_SHUT_RDWR);
