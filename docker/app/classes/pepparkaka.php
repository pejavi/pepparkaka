<?php

class Pepparkaka {

  private $socket;

  function __construct()
  {
    if ( ! extension_loaded('sockets') ) {
      die( 'The sockets extension is not loaded.' );
    }

    $this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    if ($this->socket === false)
    {
      die( "Error: socket_create() failed: reason: " .socket_strerror(socket_last_error()). " IP:".PEPPARKAKA_IP."\n" );
    }
    if (!socket_connect($this->socket, PEPPARKAKA_IP, PEPPARKAKA_PORT))
    {
      die( "Error: socket_connect() failed: reason: " .socket_strerror(socket_last_error()). " IP:".PEPPARKAKA_IP."\n" );
    }
  }

  function __destruct()
  {
    socket_close($this->socket);
  }

  public function send()
  {
    $post = json_decode( file_get_contents( "php://input" ) );
    $port = sprintf("%02d", hexdec( $post->port ) );
    $red = sprintf("%03d", hexdec( $post->red ) );
    $green = sprintf("%03d", hexdec( $post->green ) );
    $blue = sprintf("%03d", hexdec( $post->blue ) );
    if( $red == $green AND $red == $blue ) {
      $red = sprintf("%03d", 0 );
      $green = sprintf("%03d", 0 );
      $blue = sprintf("%03d", 0 );
      $white = sprintf("%03d", hexdec( $post->red ) );
    } else {
      $white = sprintf("%03d", 0 );
    }
    $command = "RGB" . $port . $red . $green . $blue . $white . "\n";
    socket_write($this->socket,$command,strlen($command));

    header( "Content-Type: application/json" );
    echo json_encode( ['success'=>TRUE,'colours'=>[$red,$green,$blue,$white]] );
  }

}
