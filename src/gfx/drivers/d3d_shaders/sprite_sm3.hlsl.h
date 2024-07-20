#define SRC(...) #__VA_ARGS__
SRC(

    struct VSInput
{
    float4 position : POSITION;
    float4 texcoord : TEXCOORD0;
    float4 color0 : COLOR0;
    float4 color1 : COLOR1;
    float4 color2 : COLOR2;
    float4 color3 : COLOR3;
    float scale : SCALING;
    float rotation : ROTATION;
	};

    struct PSInput
    {
        float4 position : SV_POSITION;
        float2 texcoord : TEXCOORD0;
        float4 color : COLOR;
    };

    PSInput VSMain(VSInput input)
	{
		PSInput output;

		// Process position
		float4 pos = float4(input.position.xy, 0.0f, 1.0f);
		pos.x = pos.x * 2.0 - 1.0;
		pos.y = 1.0 - pos.y * 2.0;

		output.position = pos;
		output.texcoord = input.texcoord;
		output.color = input.color0;
		return output;
	}

    uniform sampler s0;
    uniform Texture2D<float4> t0;

    float4 PSMain(PSInput input) : SV_TARGET
    {
        return input.color * t0.Sample(s0, input.texcoord);
    }

    float4 PSMainA8(PSInput input) : SV_TARGET
    {
        return float4(input.color.rgb, input.color.a * t0.Sample(s0, input.texcoord).a);
    }

)
