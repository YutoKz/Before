import torch
from torch import autocast
from diffusers import StableDiffusionPipeline
from diffusers.models import AutoencoderKL

hugging_token = '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' #トークン
vae = AutoencoderKL.from_pretrained("stabilityai/sd-vae-ft-ema")


ldm = StableDiffusionPipeline.from_pretrained("syaimu/7th_test",
                                              vae = vae
                                              ).to("cuda")

prompt = '((masterpiece, best quality)),((masterpiece, best quality)),(best quality) +, (masterpiece)++, (ultra detailed) ++, symmetry, 1 boy and 1 girl, (couple), ((girl on top)), ((reverse cowgirl position)), ((vaginal penis)), 1 boy, naked, faceless male, flat chest, (giant penis), testicles, thighs, lying, nsfw, 1 girl, ((naked), sitting on boys crotch, ((((spread legs)))), (((m legs))), nipples, navel, stomach, pussy, ass, (pussy juices:1.10), (cum on pussy:1.20), (bukkake:1.20), vulgarity, shiny skin, steam, sweaty, facial, ((blush)), ((embarrassed)), ((excited)), blonde hair, long hair, blue eyes (heart-shaped pupils), elf, medium breasts, (((ahegao))), outdoor, ((forest)), (cowboy shot), ((facing the front)), from outside, ((couple focus)), ((perfect face)), ((perfect anatomy)), intricate,' #<生成したい画像を表現した、文字列>

# 1000枚画像を作りたい場合
num_images = 10
for i in range(num_images):
    with autocast("cuda"):
        """
        image = ldm(prompt).images[0] # 500×500px画像が生成
        # 画像サイズを変更したい場合
        # image = ldm(prompt, height=400, width=400).images[0]
        """
        image = ldm(prompt,
                    negative_prompt='(worst quality:1.4), (low quality:1.4) , (monochrome:1.1),lowres, bad anatomy, bad hands, text, error, missing fingers, extra digit, fewer digits, cropped, worst quality, normal quality, jpeg artifacts, signature, watermark, username, blurry, bad feet. (low quality, worst quality)1.4, (bad anatomy) +, (inaccurate limb)1.3, bad composition, inaccurate eyes, extra digit, fewer digits, (extra arms)1.2, logo, (extra leg), miss arms, miss legs, pubic hair, twin tails,',  # negativeプロンプト
                    height=400,  # 縦サイズ(px)
                    width=400,  # 横サイズ(px)
                    guidance_scale=7.5,  # プロンプトの重み（生成画像の類似度（0〜20)）
                    num_inference_steps=50,  # 画像生成に費やすステップ数
                    ).images[0]
        

    # save images (本コードでは、直下に画像が生成されていきます。)
    image.save(f"./image_{i}.png")

    



