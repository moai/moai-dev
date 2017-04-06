require ( 'expression' )
require ( 'matrix' )

exp = expression

--print ( exp.toComponent ( 0 ))
--print ( exp.toComponent ( 1 ))
--print ( exp.toComponent ( 0.5 ))
--print ( exp.toComponent ( 'hello' ))
--print ( exp.toComponent ( nil ))

--[[
mtxA = matrix.matrix4x4 (
	'a11', 'a12', 'a13', 'a14',
	'a21', 'a22', 'a23', 'a24',
	'a31', 'a32', 'a33', 'a34',
	'a41', 'a42', 'a43', 'a44'
)

mtxB = matrix.matrix4x4 (
	'b11', 'b12', 'b13', 'b14',
	'b21', 'b22', 'b23', 'b24',
	'b31', 'b32', 'b33', 'b34',
	'b41', 'b42', 'b43', 'b44'
)
]]--

piv = matrix.translate ( '-pivX', '-pivY', '-pivZ' )
shr = matrix.shear ( 'scaleX', 'shearYX', 'shearZX', 'scaleY', 'shearXY', 'shearZY', 'scaleZ', 'shearXZ', 'shearYZ' )
scl = matrix.scale ( 'scaleX', 'scaleY', 'scaleZ' )
rotX = matrix.rotateX ( 'rx' )
rotY = matrix.rotateY ( 'ry' )
rotZ = matrix.rotateZ ( 'rz' )
trn = matrix.translate ( 'tx', 'ty', 'tz' )

labels = {
	'mtx.m [ ZLAffine3D::C0_R0 ]',
	'mtx.m [ ZLAffine3D::C0_R1 ]',
	'mtx.m [ ZLAffine3D::C0_R2 ]',
	nil,

	'mtx.m [ ZLAffine3D::C1_R0 ]',
	'mtx.m [ ZLAffine3D::C1_R1 ]',
	'mtx.m [ ZLAffine3D::C1_R2 ]',
	nil,

	'mtx.m [ ZLAffine3D::C2_R0 ]',
	'mtx.m [ ZLAffine3D::C2_R1 ]',
	'mtx.m [ ZLAffine3D::C2_R2 ]',
	nil,

	'mtx.m [ ZLAffine3D::C3_R0 ]',
	'mtx.m [ ZLAffine3D::C3_R1 ]',
	'mtx.m [ ZLAffine3D::C3_R2 ]',
	nil,
}

print ( 'SCL ROT_XYZ TRN')
matrix.chain ( scl, rotX, rotY, rotZ, trn ):printColumns ( labels )

print ( 'SCL ROT_XZY TRN')
matrix.chain ( scl, rotX, rotZ, rotY, trn ):printColumns ( labels )

print ( 'SCL ROT_YXZ TRN')
matrix.chain ( scl, rotY, rotX, rotZ, trn ):printColumns ( labels )

print ( 'SCL ROT_YZX TRN')
matrix.chain ( scl, rotY, rotZ, rotX, trn ):printColumns ( labels )

print ( 'SCL ROT_ZXY TRN')
matrix.chain ( scl, rotZ, rotX, rotY, trn ):printColumns ( labels )

print ( 'SCL ROT_ZYX TRN')
matrix.chain ( scl, rotZ, rotY, rotX, trn ):printColumns ( labels )
