ffmpeg -i badapplequare.mp4 -vf scale=16:16 out/%05d.bmp

i=0; 
for f in ./out/*; 
do 
    d=./out/$(printf %05d $((i/255+1))); 
    mkdir -p $d; 
    mv "$f" $d; 
    let i++; 
done
