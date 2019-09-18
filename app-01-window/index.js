(function() {

  var socket = io('127.0.0.1:17845');

  var els = document.querySelectorAll('.app');

  els.forEach(function(el) {

    var app = el.dataset.app;

    el.addEventListener('click', function(event) {

      event.preventDefault();

      socket.emit('app', app);

    });

  });

})();
