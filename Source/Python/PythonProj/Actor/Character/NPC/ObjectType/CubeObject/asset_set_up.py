import unreal

# アセットのパスを指定
skeletal_mesh_path = '/Game/OriginalContent/Mesh/Character/NPC/TypeObject/CubeObject/CubeObject'

# アセットをロード
skeletal_mesh = unreal.EditorAssetLibrary.load_asset(skeletal_mesh_path)

# ボーンソケットを追加
socket_name = 'MySocket'
bone_name = 'Bone_001'
transform = unreal.Transform()
new_socket = unreal.SkeletalMeshSocket()
new_socket.set_editor_property("SocketName","MySocketName")
new_socket.set_editor_property("BoneName","Bone_007_end")
skeletal_mesh.add_socket(new_socket)

# アセットを保存
unreal.EditorAssetLibrary.save_asset(skeletal_mesh_path)
