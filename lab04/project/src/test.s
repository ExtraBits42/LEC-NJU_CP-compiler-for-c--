.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra


dist:
	lw $t1, 36($sp)
	lw $t2, 28($sp)
	sub $t0, $t1, $t2
	sw $t0, 0($sp)
	lw $t0, 0($sp)
	sw $t0, 4($sp)
	lw $t1, 0($sp)
	lw $t2, 4($sp)
	mul $t0, $t1, $t2
	sw $t0, 8($sp)
	lw $t1, 40($sp)
	lw $t2, 32($sp)
	sub $t0, $t1, $t2
	sw $t0, 12($sp)
	lw $t0, 12($sp)
	sw $t0, 16($sp)
	lw $t1, 12($sp)
	lw $t2, 16($sp)
	mul $t0, $t1, $t2
	sw $t0, 20($sp)
	lw $t1, 8($sp)
	lw $t2, 20($sp)
	add $t0, $t1, $t2
	sw $t0, 24($sp)
	lw $v0, 24($sp)
	jr $ra

check:
	li $t0, 0
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 4($sp)
	li $t0, 0
	sw $t0, 8($sp)
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	bgt $t0, $t1, label5
	j label1
label5:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 12($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 16($sp)
	lw $t0, 12($sp)
	lw $t1, 16($sp)
	beq $t0, $t1, label4
	j label1
label4:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 20($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 24($sp)
	lw $t0, 20($sp)
	lw $t1, 24($sp)
	beq $t0, $t1, label3
	j label1
label3:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 28($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 52($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 52($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 84($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 84($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 32($sp)
	lw $t0, 28($sp)
	lw $t1, 32($sp)
	beq $t0, $t1, label2
	j label1
label2:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 36($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 76($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 68($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -28
	jal dist
	addi $sp, $sp, 44
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 40($sp)
	lw $t0, 36($sp)
	lw $t1, 40($sp)
	beq $t0, $t1, label0
	j label1
label0:
	li $t0, 1
	sw $t0, 0($sp)
label1:
	lw $v0, 0($sp)
	jr $ra

main:
	la $t0, 0($sp)
	sw $t0, 8($sp)
	la $t0, 12($sp)
	sw $t0, 20($sp)
	la $t0, 24($sp)
	sw $t0, 32($sp)
	la $t0, 36($sp)
	sw $t0, 44($sp)
	lw $t0, 8($sp)
	sw $t0, 48($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 52($sp)
	lw $t0, 48($sp)
	lw $t1, 52($sp)
	sw $t1 0($t0)
	lw $t1, 8($sp)
	addi $t0, $t1, 4
	sw $t0, 56($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 60($sp)
	lw $t0, 56($sp)
	lw $t1, 60($sp)
	sw $t1 0($t0)
	lw $t0, 20($sp)
	sw $t0, 64($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 68($sp)
	lw $t0, 64($sp)
	lw $t1, 68($sp)
	sw $t1 0($t0)
	lw $t1, 20($sp)
	addi $t0, $t1, 4
	sw $t0, 72($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 76($sp)
	lw $t0, 72($sp)
	lw $t1, 76($sp)
	sw $t1 0($t0)
	lw $t0, 32($sp)
	sw $t0, 80($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 84($sp)
	lw $t0, 80($sp)
	lw $t1, 84($sp)
	sw $t1 0($t0)
	lw $t1, 32($sp)
	addi $t0, $t1, 4
	sw $t0, 88($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 92($sp)
	lw $t0, 88($sp)
	lw $t1, 92($sp)
	sw $t1 0($t0)
	lw $t0, 44($sp)
	sw $t0, 96($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 100($sp)
	lw $t0, 96($sp)
	lw $t1, 100($sp)
	sw $t1 0($t0)
	lw $t1, 44($sp)
	addi $t0, $t1, 4
	sw $t0, 104($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 108($sp)
	lw $t0, 104($sp)
	lw $t1, 108($sp)
	sw $t1 0($t0)
	lw $t1, 44($sp)
	addi $t0, $t1, 4
	sw $t0, 112($sp)
	lw $t0, 112($sp)
	lw $t0, 0($t0)
	sw $t0, 116($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 120($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 52($sp)
	sw $t0, 128($sp)
	lw $t0, 128($sp)
	lw $t0, 0($t0)
	sw $t0, 132($sp)
	lw $t0, 132($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 44($sp)
	addi $t0, $t1, 4
	sw $t0, 140($sp)
	lw $t0, 140($sp)
	lw $t0, 0($t0)
	sw $t0, 144($sp)
	lw $t0, 144($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 48($sp)
	sw $t0, 152($sp)
	lw $t0, 152($sp)
	lw $t0, 0($t0)
	sw $t0, 156($sp)
	lw $t0, 156($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 40($sp)
	addi $t0, $t1, 4
	sw $t0, 164($sp)
	lw $t0, 164($sp)
	lw $t0, 0($t0)
	sw $t0, 168($sp)
	lw $t0, 168($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 44($sp)
	sw $t0, 176($sp)
	lw $t0, 176($sp)
	lw $t0, 0($t0)
	sw $t0, 180($sp)
	lw $t0, 180($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 36($sp)
	addi $t0, $t1, 4
	sw $t0, 188($sp)
	lw $t0, 188($sp)
	lw $t0, 0($t0)
	sw $t0, 192($sp)
	lw $t0, 192($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 40($sp)
	sw $t0, 200($sp)
	lw $t0, 200($sp)
	lw $t0, 0($t0)
	sw $t0, 204($sp)
	lw $t0, 204($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -44
	jal check
	addi $sp, $sp, 76
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 176($sp)
	lw $t0, 176($sp)
	sw $t0, 180($sp)
	lw $t1, 44($sp)
	addi $t0, $t1, 4
	sw $t0, 184($sp)
	lw $t0, 184($sp)
	lw $t0, 0($t0)
	sw $t0, 188($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 192($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 52($sp)
	sw $t0, 200($sp)
	lw $t0, 200($sp)
	lw $t0, 0($t0)
	sw $t0, 204($sp)
	lw $t0, 204($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 32($sp)
	addi $t0, $t1, 4
	sw $t0, 212($sp)
	lw $t0, 212($sp)
	lw $t0, 0($t0)
	sw $t0, 216($sp)
	lw $t0, 216($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 36($sp)
	sw $t0, 224($sp)
	lw $t0, 224($sp)
	lw $t0, 0($t0)
	sw $t0, 228($sp)
	lw $t0, 228($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 52($sp)
	addi $t0, $t1, 4
	sw $t0, 236($sp)
	lw $t0, 236($sp)
	lw $t0, 0($t0)
	sw $t0, 240($sp)
	lw $t0, 240($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 56($sp)
	sw $t0, 248($sp)
	lw $t0, 248($sp)
	lw $t0, 0($t0)
	sw $t0, 252($sp)
	lw $t0, 252($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 36($sp)
	addi $t0, $t1, 4
	sw $t0, 260($sp)
	lw $t0, 260($sp)
	lw $t0, 0($t0)
	sw $t0, 264($sp)
	lw $t0, 264($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 40($sp)
	sw $t0, 272($sp)
	lw $t0, 272($sp)
	lw $t0, 0($t0)
	sw $t0, 276($sp)
	lw $t0, 276($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -44
	jal check
	addi $sp, $sp, 76
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 248($sp)
	lw $t0, 248($sp)
	sw $t0, 252($sp)
	lw $t1, 32($sp)
	addi $t0, $t1, 4
	sw $t0, 256($sp)
	lw $t0, 256($sp)
	lw $t0, 0($t0)
	sw $t0, 260($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	lw $t0, 264($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 40($sp)
	sw $t0, 272($sp)
	lw $t0, 272($sp)
	lw $t0, 0($t0)
	sw $t0, 276($sp)
	lw $t0, 276($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 56($sp)
	addi $t0, $t1, 4
	sw $t0, 284($sp)
	lw $t0, 284($sp)
	lw $t0, 0($t0)
	sw $t0, 288($sp)
	lw $t0, 288($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 60($sp)
	sw $t0, 296($sp)
	lw $t0, 296($sp)
	lw $t0, 0($t0)
	sw $t0, 300($sp)
	lw $t0, 300($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 40($sp)
	addi $t0, $t1, 4
	sw $t0, 308($sp)
	lw $t0, 308($sp)
	lw $t0, 0($t0)
	sw $t0, 312($sp)
	lw $t0, 312($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 44($sp)
	sw $t0, 320($sp)
	lw $t0, 320($sp)
	lw $t0, 0($t0)
	sw $t0, 324($sp)
	lw $t0, 324($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t1, 36($sp)
	addi $t0, $t1, 4
	sw $t0, 332($sp)
	lw $t0, 332($sp)
	lw $t0, 0($t0)
	sw $t0, 336($sp)
	lw $t0, 336($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 40($sp)
	sw $t0, 344($sp)
	lw $t0, 344($sp)
	lw $t0, 0($t0)
	sw $t0, 348($sp)
	lw $t0, 348($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -44
	jal check
	addi $sp, $sp, 76
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	sw $v0, 320($sp)
	lw $t0, 320($sp)
	sw $t0, 324($sp)
	li $t0, 0
	sw $t0, 328($sp)
	li $t0, 0
	sw $t0, 332($sp)
	lw $t0, 180($sp)
	lw $t1, 332($sp)
	bne $t0, $t1, label6
	j label9
label9:
	li $t0, 0
	sw $t0, 336($sp)
	lw $t0, 252($sp)
	lw $t1, 336($sp)
	bne $t0, $t1, label6
	j label8
label8:
	li $t0, 0
	sw $t0, 340($sp)
	lw $t0, 324($sp)
	lw $t1, 340($sp)
	bne $t0, $t1, label6
	j label7
label6:
	li $t0, 1
	sw $t0, 328($sp)
label7:
	lw $a0, 328($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t0, 0
	sw $t0, 344($sp)
	lw $v0, 344($sp)
	jr $ra
