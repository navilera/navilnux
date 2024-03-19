//------------------------------------------------------------------------------
// 파 일 명 : mem_map.h
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 램 맵핑 정의에 관련된 헤더 
// 작 성 자 : 유영창
// 작 성 일 : 2003년 5월 21일
// 수 정 일 : 2003-10-16	PCB Ver 1.1 에 맞게 수정
//                              - BackLight 핀 삭제
// 주    의 : 
//------------------------------------------------------------------------------

#ifndef _MEM_MAP_HEADER_
#define _MEM_MAP_HEADER_

#define LINUX_MACH_TYPE                 303

#define BANK0_START                     0xA0000000              // 뱅크0 시작 위치
#define BANK0_SIZE                      (64*1024*1024)          // 뱅크0 크기 

#define DEFAULT_nCS_CS8900_BASE         (PXA_CS0_PHYS+0x00400000 ) 

#define DEFAULT_nCS_NAND_BASE           (PXA_CS1_PHYS+0x00000000 ) 
#define DEFAULT_nCS_MK712_BASE          (PXA_CS1_PHYS+0x00400000 ) 

#define DEFAULT_FLASH_BOOT              0x00000000              // 플래쉬에서 부트 시작 어드레스 
#define DEFAULT_BOOT_SIZE              	(128*1024)		// 부트영역의 크기
#define DEFAULT_BOOT_PARAM         	(DEFAULT_FLASH_BOOT+DEFAULT_BOOT_SIZE)	// 플래쉬에서 부트 파라메터 영역
#define	DEFAULT_BOOT_PARAM_SIZE		(2*1024)
							   	
                                   
#define DEFAULT_RAM_BOOT_START          0xA0F00000              // 램에서 부트     시작 어드레스 
#define DEFAULT_RAM_BOOT_PARAMS         0xA0000100              // 커널 부트 파라메터 영역   
#define DEFAULT_RAM_KERNEL_START        0xA0008000              // 램에서 커널     시작 어드레스 
#define DEFAULT_RAM_RAMDISK_START       0xA0800000              // 램에서 램디스크 시작 어드레스 
#define DEFAULT_RAM_WORK_START          0xA1000000              // 일반적인 부트 로더 작업 영역 

#define DEFAULT_RAM_KERNEL_ZERO_PAGE    0xA0000000              // 부트로더에서 커널로 전달하는 영역 시작 어드레스 


#define DEFAULT_RAM_KERNEL_IMG_SIZE    	(  1*1024*1024)
#define DEFAULT_RAM_RAMDISK_IMG_SIZE   	(  4*1024*1024)
#define DEFAULT_RAM_RAMDISK_SIZE       	(  8*1024*1024)

#define DEFAULT_PARAM_SIZE             	(  2*1024)


#endif //_MEM_MAP_HEADER_

