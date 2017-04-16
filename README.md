# PPM-Editor
Compilation-
`g++ -o ppm.exe ppmreader.cpp -std=c++11 `
Reading PPM file
`Image ppmimage("texture.ppm");`
Or
`Image ppmimage;
 ppmimage.readppm("texture.ppm");`
 
Write PPM files-
 `Image ppmimage(800,600,255,,Rgb::White);
  ppmimage.writeppm("new.ppm");`
  
Support some premade colors-
  `Rgb::Red
  Rgb::Blue
  Rgb::Black
  Rgb::White 
  Rgb::Materialblue 
  Rgb::MaterialRed `
  
 Error Handling supported
 For Hex to rgb use
 `Rgb().hex<0x000000>();` which is at compile time.
 
 More features coming soon...
  
  
  
