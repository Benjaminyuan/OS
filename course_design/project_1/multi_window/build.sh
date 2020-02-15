echo $1 
gcc  $1  -o  main  `pkg-config --cflags --libs gtk+-2.0`