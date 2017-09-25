CharsTo(pic_request(250),'uint16',2:3)

pic_send([81 0])
pic_send([81 1])
for i=1:100*10
    pic_request([83 0]);    
    pic_request([82 1]);
%    pause(0.01);
end
pic_send([81 0])    
pic_send([81 1])    
    
    

pic_request(100)

pic_send(101)

dx=400;dy=200;d0=dx-dy;d1=-dx-dy;
pic_request([102 ToChars(d0,'int16') ToChars(d1,'int16') ])

dx=0;dy=-200;d0=dx-dy;d1=-dx-dy;
pic_request([102 ToChars(d0,'int16') ToChars(d1,'int16') ])

dx=-400;dy=200;d0=dx-dy;d1=-dx-dy;
pic_request([102 ToChars(d0,'int16') ToChars(d1,'int16') ])

dx=0;dy=-200;d0=dx-dy;d1=-dx-dy;
pic_request([102 ToChars(d0,'int16') ToChars(d1,'int16') ])


