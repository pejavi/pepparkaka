<?php

class View {

  public function run()
  {
    header( "Access-Control-Allow-Origin: *" );
    require( "views/home.html" );
  }

}
