#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

struct Rgb{

	uint8_t r,g,b;
	Rgb(uint8_t r = 0,uint8_t g = 0,uint8_t b = 0) : r{ r }, g{ g }, b{ b } { }
	Rgb(const Rgb& other) : Rgb(other.r , other.g , other.b) { }
	Rgb(const Rgb&& other): Rgb(other.r , other.g , other.b) { }

	inline friend std::ostream& operator<<(std::ostream& os,const Rgb& rhs){
	 char buffer[4];
	 buffer[0] = std::min(255,(int)rhs.r);
	 buffer[1] = std::min(255,(int)rhs.g);
	 buffer[2] = std::min(255,(int)rhs.b);
	 buffer[3] = '\0';
	 os.write(buffer,3);
	}
	inline friend std::istream& operator>>(std::istream& is,Rgb& rhs){ is.read( reinterpret_cast<char*>(&rhs) , sizeof(rhs)); }
	Rgb operator + (const Rgb& rhs){ return Rgb(r + rhs.r , g + rhs.g , b + rhs.b); }
	Rgb operator * (const Rgb& rhs){ return Rgb((r * rhs.r)/255,(g * rhs.g)/255,(b * rhs.b)/255); }
	Rgb operator - (const Rgb& rhs){ return Rgb(std::max(0,r-rhs.r),std::max(0,g-rhs.g),std::max(0,b-rhs.b)); }
	Rgb operator / (const Rgb& rhs){
		if(rhs.r ==0 || rhs.g ==0 || rhs.b ==0)
			return *this;
		else
			return Rgb(r / rhs.r,g / rhs.g , b / rhs.b);
	}
	Rgb& operator += (const Rgb& rhs) { *this = *this + rhs; return *this; }
  	Rgb& operator *= (const Rgb& rhs) { *this = *this * rhs; return *this; }
  	Rgb& operator /= (const Rgb& rhs) { *this = *this / rhs; return *this; }
  	Rgb& operator -= (const Rgb& rhs) { *this = *this - rhs; return *this; }
	friend bool operator ==(const Rgb& lhs, const Rgb& rhs) { return (lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b); }
  	friend bool operator !=(const Rgb& lhs, const Rgb& rhs) { return (lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b); }
  	Rgb& operator = (const Rgb& rhs){
  		if(*this==rhs) return *this;
  		this->r = rhs.r;
  		this->g = rhs.g;
  		this->b = rhs.b;
  		return *this;
  	}
  	template<const int _hex> 
  	constexpr Rgb hex() const noexcept{ return Rgb((_hex & 0xff0000)>>16,(_hex & 0x00ff00)>>8,(_hex & 0x0000ff)); }
  	// For Normalization
  	friend float& operator += (float &f, const Rgb& color){ 
    f += (color.r + color.g + color.b) / 3.0f; 
    return f; 
  } 
  static Rgb Red,Blue,Black,White,Materialblue,MaterialRed;
};

Rgb Rgb::Red =  { 255,0,0 };
Rgb Rgb::Blue = { 0,0,255 };
Rgb Rgb::Black = { 0,0,0 };
Rgb Rgb::White = { 255,255,255 };
Rgb Rgb::Materialblue = { 33,150,243 };
Rgb Rgb::MaterialRed = { 244,67,54 };

class Image{

	uint32_t w,h,max_c;
	std::vector<Rgb> pixels;
public:
	Image() : w{ 800 } , h{ 600 } , max_c{ 255 } { }
	Image(const uint32_t& w,const uint32_t& h,const uint32_t& max_c,const Rgb& pixel) : w{w},h{h},max_c{max_c}{
		for(int i = 0 ; i < w*h ; i++) pixels.emplace_back(pixel);
	}
	Image(const Image& rhs): w{rhs.w},h{rhs.h},max_c{rhs.max_c},pixels{rhs.pixels}{ }
	explicit Image(const Image&& rhs):w{rhs.w},h{rhs.h},max_c{rhs.max_c},pixels{rhs.pixels}{ }
	Image(const std::string& fname){ readppm(fname); }

	auto begin(){ return pixels.begin(); }
  	auto end(){ return pixels.end(); }
  	auto size(){ return pixels.size(); }

	void destroy() noexcept{
		w = 800;
		h = 600;
		max_c = 255;
		pixels.erase(pixels.end());
	}
	void readppm(const std::string& imagefile) {  
    	std::ifstream file(imagefile.c_str(), std::ios::binary);
    	try{
    		if (file.fail())
      			throw "Image File Not Found\n";
    		std::string header;
    		file >> header;
    		if (header != "P6")
      			throw "Different Image Format";
    		if (!(file>>w>>h>>max_c)) {
      			throw "Invalid...";
      			destroy();
    		}
    		if(max_c!=255){
    			throw "Unsupported Bit depth";
    			destroy();
    		}
    	}catch(const char* ec){ 
    		std::cout<<"Error:"<<ec<<'\n';
    		return; 
    	}
    	file.get();
    	for(int i = 0 ; i < w*h ; ++i){
    		Rgb temp;
    		file>>temp;
    		pixels.emplace_back(temp);
    	}
  	}
  	void writeppm(const std::string& name) const{
  		try{
  			if(pixels.empty()) throw "Empty data";
  			std::ofstream file(name.c_str(),std::ios::binary);
  			file<<"P6\n"<<w<<' '<<h<<'\n'<<max_c<<'\n';
  			for(auto &pixel:pixels) file<<pixel;
  		}catch(const char* e){
  			std::cout<<"Error:"<<e<<'\n';
  			return;
  		}
  	}
  	const uint32_t& width() const noexcept{
  		return w;
  	} 
  	const uint32_t& height() const noexcept{
  		return h;
  	} 
  	Rgb& operator[](const uint32_t& n) {
  		return pixels[n];
  	}
  	Rgb& operator()(const uint32_t& x,uint32_t& y){
  		return pixels.at(x * w + y);
  	}
  	friend operator == (const Image& lhs,const Image& rhs){
  		return (lhs.w==rhs.w && lhs.h==rhs.h && lhs.max_c==rhs.max_c && lhs.pixels==rhs.pixels);
  	}
  	Image operator = (const Image& rhs){
  		if(*this == rhs) return *this;
  		w = rhs.w;
  		h = rhs.h;
  		max_c = rhs.max_c;
  		pixels.reserve(rhs.pixels.size());
  		pixels = rhs.pixels;
  		return *this;
  	}
  	Image operator * (const float& val){
  		Image temp (*this);
  		uint8_t pixel = val * 255;
  		for(auto &pix:temp) pix *= Rgb(pixel,pixel,pixel);
  		return temp;
  	}
  	Image operator + (const float& val){
  		Image temp (*this);
  		uint8_t pixel = val * 255;
  		for(auto &pix:temp) pix += Rgb(pixel,pixel,pixel);
  		return temp;
  	}
  	Image operator / (const float& val){
  		Image temp (*this);
  		uint8_t pixel = val * 255;
  		for(auto &pix:temp) pix /= Rgb(pixel,pixel,pixel);
  		return temp;
  	}
  	Image operator - (const float& val){
  		Image temp (*this);
  		uint8_t pixel = val * 255;
  		for(auto &pix:temp) pix -= Rgb(pixel,pixel,pixel);
  		return temp;
  	}
  	Image operator + (const Rgb& rhs){
  		Image temp (*this);
  		for(auto &pix:temp) pix += rhs;
  		return temp;
  	}
  	Image operator - (const Rgb& rhs){
  		Image temp (*this);
  		for(auto &pix:temp) pix -= rhs;
  		return temp;
  	}
  	Image operator * (const Rgb& rhs){
  		Image temp (*this);
  		for(auto &pix:temp) pix *= rhs;
  		return temp;
  	}
  	Image operator / (const Rgb& rhs){
  		Image temp (*this);
  		for(auto &pix:temp) pix /= rhs;
  		return temp;
  	}
  	Image operator + (const Image& rhs){
  		if(w!=rhs.w || h!=rhs.h || max_c!=rhs.max_c) return *this;
  		Image temp (*this);
  		for(int i = 0 ; i < w*h ; i++) temp[i] = pixels[i] + rhs.pixels[i];
  		return temp;
  	}
  	Image operator * (const Image& rhs){
  		if(w!=rhs.w || h!=rhs.h || max_c!=rhs.max_c) return *this;
  		Image temp (*this);
  		for(int i = 0 ; i < w*h ; i++) temp[i] = pixels[i] * rhs.pixels[i];
  		return temp;
  	}
  	Image operator - (const Image& rhs){
  		if(w!=rhs.w || h!=rhs.h || max_c!=rhs.max_c) return *this;
  		Image temp (*this);
  		for(int i = 0 ; i < w*h ; i++) temp[i] = pixels[i] - rhs.pixels[i];
  		return temp;
  	}
  	Image operator / (const Image& rhs){
  		if(w!=rhs.w || h!=rhs.h || max_c!=rhs.max_c) return *this;
  		Image temp (*this);
  		for(int i = 0 ; i < w*h ; i++) temp[i] = pixels[i] / rhs.pixels[i];
  		return temp;
  	}
  	void display_pixels_info() const noexcept{
  		int j = 0;
  		for(auto& i:pixels)
			std::cout<<"Pixel:"<<++j<<"  R:"<<int(i.r)<<" G:"<<int(i.g)<<" B:"<<int(i.b)<<'\n';
  	}
};

int main(){

	Image image(800,600,255,Rgb::White);	
	image = image + Rgb::MaterialRed;
	
	for(int j = 0 ; j < 70 ; j++){
	 	for(int i = 0 ; i < 3600 ; i++){
      		int x = (200 + j * 1.5) + 100 * cos((2*3.14*i)/3600);
      		int y = (200 + j * 1.5) + 100 * sin((2*3.14*i)/3600);

      		image[(image.height() - y) * image.width() + x] = Rgb::White;
    		}
	}
    	image.writeppm("circle.ppm");	
	std::cin.get();	
}
