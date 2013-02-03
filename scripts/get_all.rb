#!/usr/bin/ruby

room_temp_line = `/home/pi/fishy_site/scripts/get_temp`
regex = Regexp.new(/t=([\d\.]*) crccheck=(\d)/)
regex.match(room_temp_line)

if $2 then
  room_temp  = $1
else
  root_temp = 255
end

tank_temp_line  = `cat /sys/bus/w1/devices/28-0000043a8a43/w1_slave`
regex = Regexp.new(/.*: crc=[\d\w]* (\w*)\n.*t=(\d*)/)
regex.match(tank_temp_line)

if $1.eql?("YES")
  tank_temp = ($2.to_i / 1000.0).round(1)
else
  tank_temp = 255
end

lights_on = `gpio read 0`.chomp

`fswebcam -r 960x720 -d /dev/video0 /home/pi/fishy_site/public/img/Fish.jpeg`

File.open("/home/pi/fishy_site/data", 'w') {|f| f.write("#{room_temp}|#{tank_temp}|#{lights_on}") }

#puts "Room Temp: #{room_temp} Tank Temp: #{tank_temp} Light On: #{lights_on}\n"
