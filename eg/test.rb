#! /usr/bin/env ruby
require 'window'
require 'sound'
require 'resource'
require 'graphics'
#require '../input/debug/input'
require '../bin/input'

window = Window.new
window.create

resource = Resource.new('../Sound')
graphics = Graphics.new( window )
input = Input.new( window )

class IntervalActionListener
  def initialize(input, graphics, resource)
    @input = input
    @graphics = graphics
    @resource = resource
    @graphics.regist_texture(32, resource.get('test.jpg'))
    @graphics.regist_texture(64, resource.get('arrow.bmp'))
    @graphics.regist_sprite(16, 32,0, 0, 256, 256)
    @graphics.regist_sprite(1, 64, 0, 0, 32, 32 )
    @graphics.regist_sprite(2, 64, 32, 0, 32, 32 )
    @graphics.regist_sprite(3, 64, 64, 0, 32, 32 )
    @graphics.regist_sprite(4, 64, 96, 0, 32, 32 )
    @x = 0
  end
  def action
    @input.update
    @graphics.begin(0, 0, 0)
    @graphics.draw_sprite(16, @x, 100, 0)    
    if @input.down?('joy up') or @input.down?('key up') then
      @graphics.draw_sprite(1, 128, 128, 0)
    end
    if @input.down?('joy down') or @input.down?('key down') then
      @graphics.draw_sprite(2, 128, 128, 0)
    end
    if @input.down?('joy left') or @input.down?('key left') then
      @graphics.draw_sprite(3, 128, 128, 0)
    end
    if @input.down?('joy right') or @input.down?('key right')  then
      @graphics.draw_sprite(4, 128, 128, 0)
    end
    


    @graphics.end
    @x = (@x + 1) % 100
    sleep(0.001)
  end
end

action = IntervalActionListener.new(input, graphics, Resource.new('../Graphics') )



window.set_interval_action_listener(action)






sound = Sound.new( window )
sound.load(1, resource.get('smoke_l.mp3') )
#sound.load(2, resource.get('smoke_l.wav') )
sound.load(2, resource.get('smoke_l.wav') )

sound.play_with_fade(1, 6000, true)
sound.play_with_fade(2, 2500, true)
sound.back_scene(2500, true)
sound.back_scene(2500, true)
sound.back_scene(2500, true)


#print window.handle
window.show

window.main

