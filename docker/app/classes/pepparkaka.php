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
    $port = sprintf("%02d", $post->port );
    $red = sprintf("%03d", hexdec( $post->red ) );
    $green = sprintf("%03d", hexdec( $post->green ) );
    $blue = sprintf("%03d", hexdec( $post->blue ) );
    $mode = ( $post->mode == "blink"? "BLI":"RGB" );
    if( $red > 180 ) {
      $red = sprintf("%03d", 180 );
    }
    if( $green > 180 ) {
      $green = sprintf("%03d", 180 );
    }
    if( $blue > 180 ) {
      $blue = sprintf("%03d", 180 );
    }
    if( $red == $green AND $red == $blue ) {
      $red = sprintf("%03d", 0 );
      $green = sprintf("%03d", 0 );
      $blue = sprintf("%03d", 0 );
      $white = sprintf("%03d", hexdec( $post->red ) );
    } else {
      $white = sprintf("%03d", 0 );
    }

    $command1 = $mode . $port . ( $mode == "RGB"? $red . $green . $blue . $white : "" ) . "\n";
    socket_write($this->socket,$command1,strlen($command1));

    $command2 = "DISCONNECT\n";
    socket_write($this->socket,$command2,strlen($command2));

    header( "Content-Type: application/json" );
    echo json_encode( ['success'=>TRUE,'colours'=>[$red,$green,$blue,$white],'command'=>$command1] );
  }

}
