const Review = require('../models/Review');

exports.getReviewsByBookId = async (bookId) => {
    return await Review.find({ book: bookId });
};