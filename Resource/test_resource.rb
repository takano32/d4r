#! /usr/bin/env ruby

#
# リソースモジュールのテスト
#
require 'debug/resource'
require '../window/debug/window'
require '../sound/debug/sound'

# newで指定したパスがアーカイブでなければ、生リソースモードになる
# getで取得したファイルはString型になる。
#バイナリの事もあるので\0で止まらないように、オブジェクトのサイズを取得する事。
resource = Resource.new('./hoge')
file = resource.get('test.txt')
print file

# newで指定したパスがアーカイブならば、アーカイブモードになる
# getで同様にアーカイブから取り出せる（ようになる予定）。
resource2 = Resource.new('./fuga')
file = resource2.get('test2.txt');
print file

# 一応、動作的には成功したが、
# 解凍中プログレスバーが表示されるのは、消せない模様。
resource3 = Resource.new('./sample03.zip')
file = resource3.get('sample03.mp3')

window = Window.new
window.create
sound = Sound.new( window )
sound.load(1, 'sound\xeno.wav')

sound.play_with_fade(1, 6000, true)

#print window.handle
window.show
window.main
