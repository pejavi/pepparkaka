<?php

class Menu {

  public static $menu = [
    'Granen'=>[
      [
        'name'=>'Blinkande gran',
        'port'=>0,
        'type'=>'BW',
      ],
    ],
    'Stationshuset'=>[
      [
        'name'=>'Belysning 1',
        'port'=>1,
        'type'=>'RGB',
      ],
      [
        'name'=>'Belysning 2',
        'port'=>2,
        'type'=>'RGB',
      ],
    ],
    'Stinshuset'=>[
      [
        'name'=>'Belysning vänster',
        'port'=>6,
        'type'=>'RGB',
      ],
    ],
    'Lastbilen'=>[
      [
        'name'=>'Belysning i lastbil',
        'port'=>4,
        'type'=>'RGB',
      ],
    ],
    'Bodar'=>[
      [
        'name'=>'Trekamp 1',
        'port'=>8,
        'type'=>'RGB',
      ],
      [
        'name'=>'Trekamp 2',
        'port'=>9,
        'type'=>'RGB',
      ],
      [
        'name'=>'Korv',
        'port'=>10,
        'type'=>'RGB',
      ],
      [
        'name'=>'Ost',
        'port'=>11,
        'type'=>'RGB',
      ],
      [
        'name'=>'Boll',
        'port'=>12,
        'type'=>'RGB',
      ],
      [
        'name'=>'Kast',
        'port'=>13,
        'type'=>'RGB',
      ],
      [
        'name'=>'Lotto',
        'port'=>14,
        'type'=>'RGB',
      ],
      [
        'name'=>'Bod blommor 1',
        'port'=>15,
        'type'=>'RGB',
      ],
      [
        'name'=>'Bod blommor 2',
        'port'=>16,
        'type'=>'RGB',
      ],
    ],
    'Småhus'=>[
      [
        'name'=>'Verandan',
        'port'=>18,
        'type'=>'RGB',
      ],
      [
        'name'=>'Stugan',
        'port'=>20,
        'type'=>'RGB',
      ],
      [
        'name'=>'Tomtestugan 1',
        'port'=>22,
        'type'=>'RGB',
      ],
      [
        'name'=>'Skorstenen',
        'port'=>23,
        'type'=>'RGB',
      ],
      [
        'name'=>'Tomtestuga 2',
        'port'=>25,
        'type'=>'RGB',
      ],
    ],
    'Höghus'=>[
      [
        'name'=>'Höghus 1a',
        'port'=>29,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 1b',
        'port'=>30,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 1c',
        'port'=>31,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 1d',
        'port'=>32,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 1e',
        'port'=>33,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 2a',
        'port'=>36,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 2b',
        'port'=>37,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 2c',
        'port'=>38,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 2d',
        'port'=>39,
        'type'=>'RGB',
      ],
      [
        'name'=>'Höghus 2e',
        'port'=>40,
        'type'=>'RGB',
      ],
    ],
  ];

  public function get()
  {
    header( "Content-Type: application/json" );
    echo json_encode( self::$menu );
  }

}
