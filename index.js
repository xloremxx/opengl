(function() {

  var socket;
  var els;

  socket = io('127.0.0.1:17845');

  /* apps; */

  els = document.querySelectorAll('.app');

  els.forEach(function(el) {

    var app = el.dataset.app;

    el.addEventListener('click', function(event) {

      event.preventDefault();

      socket.emit('app', app);

    });

  });

  /* executables */

  els = document.querySelectorAll('.executable');

  els.forEach(function(el) {

    el.addEventListener('click', function(event) {

      event.preventDefault();

      socket.emit('exec', {
        command: el.dataset.command,
        cwd: el.dataset.cwd
      });

    });

  });

})();
