attribute vec2 inputTextureCoordinate;                                                                       
uniform int index;                                                                                           
uniform vec2 texOffset;                                                                              
uniform vec2 texSize;                                                                                
uniform vec2 textOffset;                                                                             
																											  																											  
varying vec2 textureCoordinate;                                                                      
vec2 getGlyphVertexCoord()                                                                                   
{                                                                                                            
	vec2 glyphCoord;                                                                                         
	glyphCoord.x = textOffset.x + 0.033 * (inputTextureCoordinate.x + float(index));                         
	glyphCoord.y = textOffset.y - 0.033 * inputTextureCoordinate.y;                                          
																										  
	return glyphCoord;                                                                                       
}                                                                                                            
																											  
void main()                                                                                                  
{                                                                                                            
	gl_Position = vec4(getGlyphVertexCoord(), 0.0, 1.0);                                                     
	textureCoordinate = texOffset+vec2(inputTextureCoordinate.x, inputTextureCoordinate.y)*texSize;          
}