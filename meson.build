project('demo', 'c')


cfg  = configuration_data()
cfg.set('DEBUG', true)
config_h = configure_file(output: 'config.h', configuration: cfg)

wlserver = dependency('wayland-server')
wlclient = dependency('wayland-client')

cc = meson.get_compiler('c')


executable('server', ['server.c', config_h], dependencies: [wlserver])
executable('client', ['client.c'], dependencies: [wlclient])
