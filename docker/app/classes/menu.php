<?php

class Menu {

  public static $menu = [
    'Granen'=>[
      [
        'name'=>'Blinkande gran',
        'port'=>1,
        'type'=>'BW',
      ],
    ],
    'Stationshuset'=>[
      [
        'name'=>'Belysning vänster',
        'port'=>1,
        'type'=>'RGB',
      ],
      [
        'name'=>'Belysning höger',
        'port'=>2,
        'type'=>'RGB',
      ],
    ],
    'Stinshuset'=>[
      [
        'name'=>'Belysning vänster',
        'port'=>3,
        'type'=>'RGB',
      ],
      [
        'name'=>'Belysning höger',
        'port'=>4,
        'type'=>'RGB',
      ],
    ],
    'Lastbilen'=>[
      [
        'name'=>'Belysning vänster',
        'port'=>5,
        'type'=>'RGB',
      ],
      [
        'name'=>'Belysning höger',
        'port'=>6,
        'type'=>'RGB',
      ],
    ],
    'Höghuset'=>[
      [
        'name'=>'Belysning vänster',
        'port'=>7,
        'type'=>'RGB',
      ],
      [
        'name'=>'Belysning höger',
        'port'=>8,
        'type'=>'RGB',
      ],
    ],
    'Höghuset'=>[
      [
        'name'=>'Belysning vänster',
        'port'=>7,
        'type'=>'BW',
      ],
    ],
  ];

  public function get()
  {
    header( "Content-Type: application/json" );
    echo json_encode( self::$menu );
  }

}
