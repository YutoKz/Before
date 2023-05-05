import torch
from torch import autocast
from diffusers import StableDiffusionPipeline

hugging_token = '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' #トークン

ldm = StableDiffusionPipeline.from_pretrained("CompVis/stable-diffusion-v1-4",
                                              revision="fp16",
                                              torch_dtype=torch.float16,
                                              use_auth_token=hugging_token
                                              ).to("cuda")

prompt = '京都の秋' #<生成したい画像を表現した、文字列>

# 1000枚画像を作りたい場合
num_images = 10
for i in range(num_images):
    with autocast("cuda"):
        image = ldm(prompt).images[0] # 500×500px画像が生成
        # 画像サイズを変更したい場合
        # image = ldm(prompt, height=400, width=400).images[0]

    # save images (本コードでは、直下に画像が生成されていきます。)
    image.save(f"./image_{i}.png")

    