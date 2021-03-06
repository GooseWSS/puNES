"struct data\n"
"{\n"
"   float2 tex;\n"
"};\n"
"\n"
"struct input\n"
"{\n"
"   float2 video_size;\n"
"   float2 texture_size;\n"
"   float2 output_size;\n"
"   float frame_count;\n"
"};\n"
"\n"
"void main_vertex\n"
"(\n"
"   float4 position : POSITION,\n"
"   out float4 oPosition : POSITION,\n"
"   uniform float4x4 modelViewProj,\n"
"   float2 tex : TEXCOORD,\n"
"   uniform input IN,\n"
"   out data oData\n"
")\n"
"{\n"
"   oPosition = mul(modelViewProj, position);\n"
"   oData.tex = tex - float2(0.5 / IN.texture_size.x, 0.0); // Compensate for decimate-by-2.\n"
"}\n"
"\n"
"\n"

