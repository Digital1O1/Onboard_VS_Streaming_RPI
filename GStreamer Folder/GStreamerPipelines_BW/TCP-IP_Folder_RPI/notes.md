# Useful commands

## To determine supprted video formats 
### Use : `v4l2-ctl --list-formats-ext`

```bash
ioctl: VIDIOC_ENUM_FMT
        Type: Video Capture

        [0]: 'YUYV' (YUYV 4:2:2)
                Size: Stepwise 16x16 - 16376x16376 with step 1/1
        [1]: 'UYVY' (UYVY 4:2:2)
                Size: Stepwise 16x16 - 16376x16376 with step 1/1
        [2]: 'YVYU' (YVYU 4:2:2)
                Size: Stepwise 16x16 - 16376x16376 with step 1/1
        [3]: 'VYUY' (VYUY 4:2:2)
                Size: Stepwise 16x16 - 16376x16376 with step 1/1
```

### Understanding 16376x16376 output
- 16376x16376 refers to the `max resolution` that's supported by the camera for that specific format
  - Important to note : The `max resolution` stated is it's theoretical resolution that actually not be supported 
- Stepwise 16x16
  - Camera supports resolutions in `increments of 16 pixels` in both the width/height 
  - Example of valid resolutions : `16x16, 32x32, 48x48`