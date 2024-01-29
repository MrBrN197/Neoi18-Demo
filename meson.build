project('demo', 'c')

wlserver = dependency('wayland-server')
wlclient = dependency('wayland-client')

cc = meson.get_compiler('c')

executable('server', ['server.c', 'wayland/wayland.public.c'], dependencies: [wlserver])
executable('client', ['client.c', 'wayland/wayland.public.c'], dependencies: [wlclient])
