const mongoose = require('../config/db');

const readerSchema = new mongoose.Schema({
    name: { type: String, required: true },
    contactInfo: { type: String, required: true },
    borrowedBooks: [{
        book: { type: mongoose.Schema.Types.ObjectId, ref: 'Book' },
        borrowDate: { type: Date, default: Date.now },
        returnDate: { type: Date },
        status: { type: String, enum: ['borrowed', 'returned'], default: 'borrowed' },
        dueDate: { type: Date }
    }]
});

module.exports = mongoose.model('Reader', readerSchema);