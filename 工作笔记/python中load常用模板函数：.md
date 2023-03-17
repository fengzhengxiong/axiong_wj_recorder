python中关于读取保存文件

demo1:dict->json

```
import json
a = {"translation_vec_mid": [0.55781,-0.280544,0.362354]}
b = json.dumps(a,indent=4)  # indent是向前缩进几个空格
f = open('C:/Users\wanji\Desktop/demos.json', 'w')
f.write(b)
f.close()
```

demo2：json->dict

```
import json
with open('C:/Users\wanji\Desktop/demos.json', encoding='utf8') as f:
    config_dict = json.load(f)
    f.close()
print(config_dict)
```

demo3：json->ymal

```
import yaml, json
with open(C:/Users\wanji\Desktop/demos.json, encoding='utf8') as f:
    config_dict = json.load(f)
    f.close()
dyaml = yaml.load(str(config_dict))  
stream = open(C:/Users\wanji\Desktop/demos.yaml, 'w')
yaml.safe_dump(dyaml, stream, default_flow_style=False) 
```

demo4：ymal->json

```
import json,yaml
test_file = open('C:/Users\wanji\Desktop/demos.yaml',"r")
generate_dict = yaml.load(test_file)
generate_json = json.dumps(generate_dict,indent=4)
f = open('C:/Users\wanji\Desktop/demos.json', 'w')
f.write(generate_json)
f.close()
```



