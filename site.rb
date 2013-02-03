require 'rubygems'
require 'sinatra'
require 'haml'

get '/' do
  data = File.new("data", "r")
  line = data.gets
  (@room_temp, @tank_temp, @light_on) = line.split("|")
  
  data.close
  
  if @light_on.eql?("0") then
    @light = "off"
  else
    @light = "on"
  end
  
  haml :index
end

get '/about' do
  haml :about
end

get '/contact' do
  haml :contact
end
