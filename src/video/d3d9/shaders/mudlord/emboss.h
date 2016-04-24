{
"/* COMPATIBILITY \n"
"   - HLSL compilers\n"
"   - Cg   compilers\n"
"*/\n"
"\n"
"struct input\n"
"{\n"
"  float2 video_size;\n"
"  float2 texture_size;\n"
"  float2 output_size;\n"
"  float frame_count;\n"
"  float frame_direction;\n"
"  float frame_rotation;\n"
"};\n"
"\n"
"\n"
"struct VERTEX_OUTPUT {\n"
"  float4 position : POSITION;\n"
"  float2 texCoord : TEXCOORD0;\n"
"};\n"
"\n"
"\n"
"VERTEX_OUTPUT main_vertex\n"
"(\n"
"	float4 position	: POSITION,\n"
"	float2 texCoord : TEXCOORD0,\n"
"\n"
"   	uniform float4x4 modelViewProj\n"
")\n"
"{\n"
"	VERTEX_OUTPUT OUT;\n"
"\n"
"	OUT.position = mul(modelViewProj, position);\n"
"	OUT.texCoord = texCoord;\n"
"\n"
"	return OUT;\n"
"}\n"
"\n"
"struct output \n"
"{\n"
"  float4 color : COLOR;\n"
"};\n"
"\n"
"\n"
"struct VERTEX_INPUT\n"
"{\n"
"	float4 position : POSITION;\n"
"	float2 CT : TEXCOORD0;\n"
"};\n"
"\n"
"\n"
"output main_fragment(in VERTEX_INPUT VAR, uniform sampler2D decal : TEXUNIT0, uniform input IN) \n"
"{\n"
"	output OUT;\n"
"\n"
"	float4 Color = tex2D( decal, VAR.CT );\n"
"        Color.a = 1.0f;\n"
"        Color.rgb = 0.5f;\n"
"        Color -= tex2D( decal, VAR.CT.xy-0.001)*2.0f;\n"
"        Color += tex2D( decal, VAR.CT.xy+0.001)*2.0f;\n"
"        Color.rgb = (Color.r+Color.g+Color.b)/3.0f;\n"
"\n"
"	OUT.color = Color;\n"
"	return OUT;\n"
"}\n"
},