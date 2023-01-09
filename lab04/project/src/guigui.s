FUNCTION dist :
PARAM d_x1
PARAM d_y1
PARAM d_x2
PARAM d_y2
temp0 := d_x2 - d_x1
temp1 := temp0
temp2 := temp0 * temp1
temp3 := d_y2 - d_y1
temp4 := temp3
temp5 := temp3 * temp4
temp6 := temp2 + temp5
RETURN temp6
FUNCTION check :
PARAM p1_x
PARAM p1_y
PARAM p2_x
PARAM p2_y
PARAM p3_x
PARAM p3_y
PARAM p4_x
PARAM p4_y
temp7 := #0
ARG p2_y
ARG p2_x
ARG p1_y
ARG p1_x
temp8 := CALL dist
temp9 := #0
IF temp8 > temp9 GOTO label5
GOTO label1
LABEL label5 :
ARG p2_y
ARG p2_x
ARG p1_y
ARG p1_x
temp10 := CALL dist
ARG p3_y
ARG p3_x
ARG p2_y
ARG p2_x
temp11 := CALL dist
IF temp10 == temp11 GOTO label4
GOTO label1
LABEL label4 :
ARG p3_y
ARG p3_x
ARG p2_y
ARG p2_x
temp12 := CALL dist
ARG p4_y
ARG p4_x
ARG p3_y
ARG p3_x
temp13 := CALL dist
IF temp12 == temp13 GOTO label3
GOTO label1
LABEL label3 :
ARG p4_y
ARG p4_x
ARG p3_y
ARG p3_x
temp14 := CALL dist
ARG p1_y
ARG p1_x
ARG p4_y
ARG p4_x
temp15 := CALL dist
IF temp14 == temp15 GOTO label2
GOTO label1
LABEL label2 :
ARG p3_y
ARG p3_x
ARG p1_y
ARG p1_x
temp16 := CALL dist
ARG p4_y
ARG p4_x
ARG p2_y
ARG p2_x
temp17 := CALL dist
IF temp16 == temp17 GOTO label0
GOTO label1
LABEL label0 :
temp7 := #1
LABEL label1 :
RETURN temp7
FUNCTION main :
DEC temp18 8
p1 := &temp18
DEC temp19 8
p2 := &temp19
DEC temp20 8
p3 := &temp20
DEC temp21 8
p4 := &temp21
temp24 := p1
READ temp26
*temp24 := temp26
temp29 := p1 + #4
READ temp31
*temp29 := temp31
temp34 := p2
READ temp36
*temp34 := temp36
temp39 := p2 + #4
READ temp41
*temp39 := temp41
temp44 := p3
READ temp46
*temp44 := temp46
temp49 := p3 + #4
READ temp51
*temp49 := temp51
temp54 := p4
READ temp56
*temp54 := temp56
temp59 := p4 + #4
READ temp61
*temp59 := temp61
temp65 := p4 + #4
temp67 := *temp65
ARG temp67
temp70 := p4
temp72 := *temp70
ARG temp72
temp75 := p3 + #4
temp77 := *temp75
ARG temp77
temp80 := p3
temp82 := *temp80
ARG temp82
temp85 := p2 + #4
temp87 := *temp85
ARG temp87
temp90 := p2
temp92 := *temp90
ARG temp92
temp95 := p1 + #4
temp97 := *temp95
ARG temp97
temp100 := p1
temp102 := *temp100
ARG temp102
temp62 := CALL check
chk_res1 := temp62
temp106 := p4 + #4
temp108 := *temp106
ARG temp108
temp111 := p4
temp113 := *temp111
ARG temp113
temp116 := p2 + #4
temp118 := *temp116
ARG temp118
temp121 := p2
temp123 := *temp121
ARG temp123
temp126 := p3 + #4
temp128 := *temp126
ARG temp128
temp131 := p3
temp133 := *temp131
ARG temp133
temp136 := p1 + #4
temp138 := *temp136
ARG temp138
temp141 := p1
temp143 := *temp141
ARG temp143
temp103 := CALL check
chk_res2 := temp103
temp147 := p3 + #4
temp149 := *temp147
ARG temp149
temp152 := p3
temp154 := *temp152
ARG temp154
temp157 := p4 + #4
temp159 := *temp157
ARG temp159
temp162 := p4
temp164 := *temp162
ARG temp164
temp167 := p2 + #4
temp169 := *temp167
ARG temp169
temp172 := p2
temp174 := *temp172
ARG temp174
temp177 := p1 + #4
temp179 := *temp177
ARG temp179
temp182 := p1
temp184 := *temp182
ARG temp184
temp144 := CALL check
chk_res3 := temp144
temp186 := #0
temp187 := #0
IF chk_res1 != temp187 GOTO label6
GOTO label9
LABEL label9 :
temp188 := #0
IF chk_res2 != temp188 GOTO label6
GOTO label8
LABEL label8 :
temp189 := #0
IF chk_res3 != temp189 GOTO label6
GOTO label7
LABEL label6 :
temp186 := #1
LABEL label7 :
WRITE temp186
temp190 := #0
RETURN temp190
