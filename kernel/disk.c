#include <litrix/portio.h>
#include <litrix/disk.h>
#include <litrix/stdout.h>

void disable_ata_irq(void) {
    outb(ATA_PRIMARY_CONTROL_BASE, 0x02);
}

void ata_wait_bsy(void) {
    while (inb(ATA_PRIMARY_IO + 7) & 0x80);
}

void ata_wait_drq(void) {
    while (!(inb(ATA_PRIMARY_IO + 7) & 0x08));
}

void ata_select_drive(unsigned char drive) {
    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0 | (drive << 4));
    ata_wait_bsy();
}

void ata_read_sector(unsigned int lba, unsigned char *buffer) {
    outb(ATA_PRIMARY_CONTROL_BASE, 0x02);
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + 1, 0x00);
    outb(ATA_PRIMARY_IO + 2, 1); // Sector count
    outb(ATA_PRIMARY_IO + 3, (unsigned char) lba);
    outb(ATA_PRIMARY_IO + 4, (unsigned char)(lba >> 8));
    outb(ATA_PRIMARY_IO + 5, (unsigned char)(lba >> 16));
    outb(ATA_PRIMARY_IO + 7, ATA_CMD_READ_PIO);

    ata_wait_bsy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        ((unsigned short*)buffer)[i] = inw(ATA_PRIMARY_IO); // Use inw instead of inb
    }
}

void ata_write_sector(unsigned int lba, const unsigned char* buffer) {
    outb(ATA_PRIMARY_CONTROL_BASE, 0x02);
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + 1, 0x00);
    outb(ATA_PRIMARY_IO + 2, 1); // Sector count
    outb(ATA_PRIMARY_IO + 3, (unsigned char) lba);
    outb(ATA_PRIMARY_IO + 4, (unsigned char)(lba >> 8));
    outb(ATA_PRIMARY_IO + 5, (unsigned char)(lba >> 16));
    outb(ATA_PRIMARY_IO + 7, ATA_CMD_WRITE_PIO);

    ata_wait_bsy();

    for (int i = 0; i < 256; i++) {
        outw(ATA_PRIMARY_IO, ((unsigned short*)buffer)[i]); // Use outw instead of outb
    }

    ata_wait_bsy();
}

unsigned char ata_disk_status(void) {
    unsigned char status = inb(ATA_PRIMARY_IO + 7);
    if (status & STATUS_ERR) {
    }
    if (status & STATUS_DF) {
    }
    if (status & STATUS_RDY) {
        status = 0;
    }

    return status;
}

unsigned char init_ata(unsigned char silent) {
    disable_ata_irq();
    ata_select_drive(0);

    if(!silent)
        printf(LITRIX_LOG "[io::ata] Initialized ATA\n");

    return ata_disk_status();
}

void ata_read(unsigned char* buff, unsigned int lba, unsigned int count) {
    unsigned char* buffer = buff;
    for(unsigned int i = 0; i < count; ++i) {
        ata_read_sector(lba+i, buffer);
        buffer += 512;
    }
}

void ata_write(const unsigned char* buff, unsigned int lba, unsigned int count) {
    for(unsigned int i = 0; i < count; ++i) {
        ata_write_sector(lba+i, buff);
        buff += 512;
    }
}
