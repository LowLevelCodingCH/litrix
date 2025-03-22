#define ATA_PRIMARY_IO 0x1F0
#define ATA_PRIMARY_CTRL 0x3F6
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_WRITE_PIO 0x30
#define ATA_REG_HDDEVSEL 0x06
#define STATUS_RDY 0x40
#define IRQ_MASK_ATA_PRIMARY 0x40 // Mask for IRQ 14 (Primary ATA)
#define IRQ_MASK_ATA_SECONDARY 0x80 // Mask for IRQ 15 (Secondary ATA)
#define ATA_PRIMARY_CONTROL_BASE 0x3F6
#define STATUS_BSY	  0x80
#define STATUS_RDY	  0x40
#define STATUS_DRQ	  0x08
#define STATUS_DF		0x20
#define STATUS_ERR	  0x01

void disable_ata_irq(void);
void ata_wait_bsy(void);
void ata_wait_drq(void);
void ata_select_drive(unsigned char drive);
void ata_read_sector(unsigned int lba, unsigned char *buffer);
void ata_write_sector(unsigned int lba, const unsigned char* buffer);
unsigned char ata_disk_status(void);
unsigned char init_ata(unsigned char silent);
void ata_read(unsigned char* buff, unsigned int lba, unsigned int count);
void ata_write(const unsigned char* buff, unsigned int lba, unsigned int count);
