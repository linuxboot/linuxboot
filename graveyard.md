# LinuxBoot Graveyard 🪦

We iteratively walk through various approaches to LinuxBoot's design.
Earlier attempts are documented below for reference. The code may still be
useful in general, though requires rework to fit into the current design.

## Removals

The following boards have been removed from the `mainboards/` directory:

| Path                              | Deleting Commit                          |
| --------------------------------- | ---------------------------------------- |
| `aeeon/i11`                       | 0217e85b040a93a373932ab30e161f5b7efe1b9e |
| `aeeon/upxtreme`                  | 85b277da4b1f59c9101117e4192b02d8fe6fee60 |
| `amd/rome`                        | cf447e193a1ee39c01f526597400d029e70784de |
| `bytedance/g220a`                 | 8a48d3eabf07c29d1a1d8233671fc456808afa8b |
| `cubie/board`                     | 03ce8a4a0558b335de812af9e34f4f5b48265756 |
| `digitalloggers/atomicpi`         | faa2b2bac902ada422479b81023816c8aea9ea77 |
| `hpe/dl360gen10`                  | 38655daa09f8e2bb95887a954e0231c1e1383076 |
| `intel/generic`                   | 36f3d42dade41f310676383b0b015a4ed1b85502 |
| `intel/hw`                        | ad41eb3e06a45360c8e47b4e69b8d64256c02c5a |
| `intel/minplatform`               | 8460a4051cd2304727937c4c6b85af43b4eecbc2 |
| `intel/s2600`                     | 0a0fe3961711c8e2394753f72d99462509d1f335 |
| `lenovo/hr630`                    | bf0c59238dd5af734b604bb53a54579ba790a04b |
| `lenovo/sr630`                    | 35b0b81242b961b8f1019f29ca49e4d8e1ad934a |
| `lenovo/thinkpad`                 | 54be995b382bf97ee56334da05cb346a91b3e723 |
| `marvel/macchiatobin`             | 09161d4f29529df3b95b01900e926ed37c8c0655 |
| `opentitanpilot/dresden`          | d7cc8e21bdc0aa986239f0b7899faaedd59de208 |
| `pcengines/apu`                   | 89dcf5e79e9587b17124de45d0d869151c8cd0a5 |
| `pcengines/apu2`                  | c127d3c19f814b312b8b243f35e754d4c8f05ef1 |
| `qemu/arm64`                      | b215d5ce1d821033094a71227b3e7846036b8425 |
| `seeed/beaglev`                   | d4b9d4dcbb61c656009646d1097d85ab335f1976 |
| `slimboot`                        | 0dc8ae7c9fe7c2dc65ba60a77169750380b5e3e8 |
| `solidrun/honeycomblx2k`          | 6825bff2474a8b18264c870524bf293f4a323fdb |
| `st/st32mp1517c`                  | dbaba8cdf9cf883d13418c5b9a127fc102d1e725 |
| `sunxi/nezha`                     | ddcd5e4e1fe266dbd98a06618ccaae9339426ce4 |
| `tianocore/ovmf`                  | 93c06bea540e6fed1c65bc884ceca5ecb002e48d |
| `tyan7106`                        | e49b38ec1fff220852be5a75f7745f7e981e543f |
| `walmart/robot`                   | a9901247ae86e1bae6fc560f75393a1f5e8b9ee6 |
