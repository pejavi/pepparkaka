<?php

class Controller {

  public function run()
  {
    switch( substr($_SERVER["REQUEST_URI"],1) ) {
      case 'api/getMenu':
        require 'classes/menu.php';
        $pepparkaka = new Menu();
        $pepparkaka->get();
      break;
      case 'api/setPepparkaka':
        require 'classes/pepparkaka.php';
        $pepparkaka = new Pepparkaka();
        $pepparkaka->send();
      break;
      default:
        require( "classes/view.php" );
        $view = new View();
        $view->run();
      break;
    }
  }

}
