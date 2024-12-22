# Parse wavefront obj file into a c struct

def parse_vertex(line):
    # Obj vertex format: v x y z
    # C format: vertex { .x = double($x), .y = double($y), .z = double($z) } 
    return f'vertex {{ .x = double({line[1]}), .y = double({line[2]}), .z = double({line[3].strip()}) }}'

def parse_normal(line):
    # Obj normal format: vn x y z
    # C format: normal { .x = double($x), .y = double($y), .z = double($z) } 
    return f'normal {{ .x = double({line[1]}), .y = double({line[2]}), .z = double({line[3].strip()}) }}'

def parse_face(line):
    # Obj face format: f v1//n1 v2//n2 v3//n3
    # C format: face { 
    #    .vertex_index = ivec3 { .x = int64_t($v1), .y = int64_t($v2), .z = int64_t($v3) }, 
    #    .normal_index = ivec3 { .x = int64_t($n1), .y = int64_t($n2), .z = int64_t($n3) }} 
    # }
    v1 = line[1].split('//')[0]
    v2 = line[2].split('//')[0]
    v3 = line[3].split('//')[0]
    n1 = line[1].split('//')[1]
    n2 = line[2].split('//')[1]
    n3 = line[3].split('//')[1].strip()
    return f'face {{ .vertex_index = ivec3 {{ .x = int64_t({v1}), .y = int64_t({v2}), .z = int64_t({v3}) }}, .normal_index = ivec3 {{ .x = int64_t({n1}), .y = int64_t({n2}), .z = int64_t({n3}) }} }}'

def parse_obj(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
        vertices = []
        normals = []
        faces = []
        for line in lines:
            if line.startswith('v'):
                vertices.append(parse_vertex(line.split(' ')))
            elif line.startswith('n'):
                normals.append(parse_normal(line.split(' ')))
            elif line.startswith('f'):
                faces.append(parse_face(line.split(' ')))
            
        return vertices, normals, faces

def parse_file(filename):
    vertices, normals, faces = parse_obj(filename)

    # Assamble c file
    c_file = \
f'''const auto teapot_model = obj_3d {{
    .vertices = {{{
        ',\n'.join(vertices)
    }}}
    .normals = {{{
        ',\n'.join(normals)
    }}}
    .faces = {{{
        ',\n'.join(faces)
    }}},
}}'''
    
    return c_file

if __name__ == '__main__':
    c_file = parse_file('teapot.obj')
    print(c_file)

