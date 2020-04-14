<?php

$socket = stream_socket_client('tcp://192.168.1.14:4444');
if ($socket) {
    $sent = stream_socket_sendto($socket, 'message');
    if ($sent > 0) {
        $server_response = fread($socket, 4096);
        echo $server_response;
    }
} else {
    echo 'Unable to connect to server';
}
stream_socket_shutdown($socket, STREAM_SHUT_RDWR);
