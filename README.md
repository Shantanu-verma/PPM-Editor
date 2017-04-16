# PPM-Editor
1.Compilation-
 `g++ -o ppm.exe ppmreader.cpp -std=c++11 `

2.Reading PPM file

 `Image ppmimage("texture.ppm");`

Or

  `Image ppmimage;`

  `ppmimage.readppm("texture.ppm");`
 
3.Write PPM files-

 `Image ppmimage(800,600,255,,Rgb::White);`
 
  `ppmimage.writeppm("new.ppm");`
  
4.Support some premade colors-

  `Rgb::Red`
  
  `Rgb::Blue`
  
  `Rgb::Black`
  
  `Rgb::White `
  
  `Rgb::Materialblue `
  
  `Rgb::MaterialRed `
  
  
 5.Error Handling supported
 
 6.For Hex to rgb use
 
  `Rgb().hex<0x000000>();` which is at compile time.
 
 7.Support Basic arithmatic between colors and images
 
 More features coming soon...
  
  
  
