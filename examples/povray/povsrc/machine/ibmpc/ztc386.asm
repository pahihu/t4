	.386p
;
;  void get_real_buf(long *real, char _far **ptr);
;
_TEXT	segment	public para use32 'CODE'
	assume	cs:_TEXT
	public	_get_real_buf
_get_real_buf proc near
	push	es			;Remember, pushes 4 bytes even if seg!
	push	esi
	mov	ax,250Dh
	int	21h
	mov	esi,dword ptr ss:4+8[esp]
	mov	dword ptr ds:[esi],ebx
	mov	esi,dword ptr ss:8+8[esp]
	mov	dword ptr ds:[esi],edx
	mov	word ptr ds:[esi+4],es
	pop	esi
	pop	es
	ret
_get_real_buf endp
_TEXT	ends
	end
