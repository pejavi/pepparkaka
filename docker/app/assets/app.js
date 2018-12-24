angular.module('pepparkakaApp', [])
.controller('PepparkakaCtrl', function($http) {
  var pepparkaka = this;

  pepparkaka.active = [
    {'menu':null},
  ];
  pepparkaka.colours = {
    '#000000':'Off',
    '#FFFFFF':'Vitt',
    '#FF00FF':'Lila',
    '#FFFF00':'Gul',
    '#FF0000':'Röd',
    '#0000FF':'Blå',
    '#00FF00':'Grön',
  };

  pepparkaka.menu = {};
  pepparkaka.getMenu = function() {
    $http({
        method : "GET",
        url : "/api/getMenu"
    }).then(function mySuccess(response) {
        pepparkaka.menu = response.data;
    }, function myError(response) {
        console.log('Error getMenu');
        console.log(response);
    });
  }
  pepparkaka.getMenu();

  pepparkaka.setPepparkaka = function( menu, red, green, blue, mode ) {
    console.log([menu.port, red, green, blue]);
    $http({
      method : "POST",
      url : "/api/setPepparkaka",
      data : {
        'port':menu.port,
        'red':red,
        'green':green,
        'blue':blue,
        'mode':mode,
      }
    }).then(function mySuccess(response) {
      if( response.data.success ) {
        menu.state = '#'+red+green+blue;
        console.log(response.data.command);
        console.log('----------');
      } else {
        console.log('Error getMenu A');
        alert( JSON.stringify( response ) );
      }
    }, function myError(response) {
        console.log('Error getMenu B');
        console.log(response);
    });
  }

});
