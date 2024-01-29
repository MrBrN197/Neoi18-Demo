project('demo', 'c')

wlserver = dependency('wayland-server')
wlclient = dependency('wayland-client')

cc = meson.get_compiler('c')

executable('server', ['server.c'], dependencies: [wlserver])
